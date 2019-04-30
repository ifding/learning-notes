package test;

import com.datastax.driver.core.Session;
import com.datastax.spark.connector.cql.CassandraConnector;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.*;
import org.apache.spark.api.java.Optional;
import org.apache.spark.api.java.function.FlatMapFunction;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.Function2;
import org.apache.spark.api.java.function.PairFlatMapFunction;
import scala.Tuple2;

import java.io.Serializable;
import java.math.BigDecimal;
import java.text.MessageFormat;
import java.util.*;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;
import static com.datastax.spark.connector.japi.CassandraJavaUtil.mapRowTo;
import static com.datastax.spark.connector.japi.CassandraJavaUtil.mapToRow;


/**
 * Created by Ken on 12/20/16.
 * How to use Spark Cassandra Connector with Java.
 */

public class Spark2Cassandra {

    public static void main(String[] args) {

        SparkConf conf = new SparkConf();
        conf.setAppName("SparkCassandra");
        conf.setMaster("local[2]");
        conf.set("spark.cassandra.connection.host", "localhost");

        JavaSparkContext sc = new JavaSparkContext(conf);

        createData(sc);
        compute(sc);
        showData(sc);

        sc.stop();
    }

    private static void createData( JavaSparkContext sc) {

        CassandraConnector connector = CassandraConnector.apply(sc.getConf());

        // prepare the schema
        try (Session session = connector.openSession()){
            session.execute("DROP KEYSPACE IF EXISTS test");
            session.execute("CREATE KEYSPACE test WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1}");
            session.execute("CREATE TABLE test.products (id INT PRIMARY KEY, name TEXT, parents LIST<INT>)");
            session.execute("CREATE TABLE test.sales (id UUID PRIMARY KEY, product INT, price DECIMAL)");
            session.execute("CREATE TABLE test.summaries (product INT PRIMARY KEY, summary DECIMAL)");
        }

        // prepare the products hierarchy
        List<Product> products = Arrays.asList(
                new Product(0, "All Products", Collections.<Integer> emptyList()),
                new Product(1, "Product A", Arrays.asList(0)),
                new Product(4, "Product A1", Arrays.asList(0, 1)),
                new Product(5, "Product A2", Arrays.asList(0, 1)),
                new Product(2, "Product B", Arrays.asList(0)),
                new Product(6, "Product B1", Arrays.asList(0, 2)),
                new Product(7, "Product B2", Arrays.asList(0, 2)),
                new Product(3, "Product C", Arrays.asList(0)),
                new Product(8, "Product C1", Arrays.asList(0, 3)),
                new Product(9, "Product C2", Arrays.asList(0, 3)));

        JavaRDD<Product> productsRDD = sc.parallelize(products);
        javaFunctions(productsRDD).writerBuilder("test", "products", mapToRow(Product.class)).saveToCassandra();

        JavaRDD<Sale> salesRDD = productsRDD.filter(
                new Function<Product, Boolean>() {

                    @Override
                    public Boolean call(Product product) throws Exception {
                        return product.getParents().size() == 2;
                    }
                }).flatMap(new FlatMapFunction<Product, Sale>() {
            @Override
            public Iterator<Sale> call(Product product) throws Exception {
                Random random = new Random();
                List<Sale> sales = new ArrayList<>(100);
                for (int i = 0; i < 100; i++) {
                    sales.add(new Sale(UUID.randomUUID(), product.getId(),
                            BigDecimal.valueOf(random.nextDouble())));
                }
                return sales.iterator();
            }
        });

        javaFunctions(salesRDD).writerBuilder("test", "sales", mapToRow(Sale.class)).saveToCassandra();
    }

    private static void compute(JavaSparkContext sc) {

        JavaPairRDD<Integer, Product> productsRDD = javaFunctions(sc)
                .cassandraTable("test", "products", mapRowTo(Product.class))
                .keyBy(new Function<Product, Integer>() {
                    @Override
                    public Integer call(Product product) throws Exception {
                        return product.getId();
                    }
                });

        JavaPairRDD<Integer, Sale> salesRDD = javaFunctions(sc)
                .cassandraTable("test", "sales", mapRowTo(Sale.class))
                .keyBy(new Function<Sale, Integer>() {
                    @Override
                    public Integer call(Sale sale) throws Exception {
                        return sale.getProduct();
                    }
                });

        JavaPairRDD<Integer, Tuple2<Sale, Product>> joinedRDD = salesRDD.join(productsRDD);

        JavaPairRDD<Integer, BigDecimal> allSalesRDD = joinedRDD
                .flatMapToPair(new PairFlatMapFunction<Tuple2<Integer, Tuple2<Sale, Product>>, Integer, BigDecimal>() {
                    @Override
                    public Iterator<Tuple2<Integer, BigDecimal>> call(
                            Tuple2<Integer, Tuple2<Sale, Product>> input) throws Exception {
                        Tuple2<Sale, Product>  saleProduct = input._2();
                        List<Tuple2<Integer, BigDecimal>> allSales = new ArrayList<Tuple2<Integer, BigDecimal>>(
                                saleProduct._2().getParents().size() + 1);
                        allSales.add(new Tuple2<>(saleProduct._1().getProduct(), saleProduct._1().getPrice()));
                        for (Integer parentProduct : saleProduct._2().getParents()) {
                            allSales.add(new Tuple2<>(parentProduct, saleProduct._1().getPrice()));
                        }
                        return allSales.iterator();
                    }
                });

        JavaRDD<Summary> summariesRDD = allSalesRDD
                .reduceByKey(new Function2<BigDecimal, BigDecimal, BigDecimal>() {
                    @Override
                    public BigDecimal call(BigDecimal v1, BigDecimal v2) throws Exception {
                        return v1.add(v2);
                    }
                }).map(new Function<Tuple2<Integer, BigDecimal>, Summary>() {
                    @Override
                    public Summary call(Tuple2<Integer, BigDecimal> input) throws Exception {
                        return new Summary(input._1(), input._2());
                    }
                });

        javaFunctions(summariesRDD).writerBuilder("test", "summaries", mapToRow(Summary.class)).saveToCassandra();
    }

    private static void showData(JavaSparkContext sc) {

        JavaPairRDD<Integer, Summary> summariesRDD = javaFunctions(sc)
                .cassandraTable("test", "summaries", mapRowTo(Summary.class))
                .keyBy(new Function<Summary, Integer>() {
                    @Override
                    public Integer call(Summary summary) throws Exception {
                        return summary.getProduct();
                    }
                });

        JavaPairRDD<Integer, Product> productsRDD = javaFunctions(sc)
                .cassandraTable("test", "products", mapRowTo(Product.class))
                .keyBy(new Function<Product, Integer>() {
                    @Override
                    public Integer call(Product product) throws Exception {
                        return product.getId();
                    }
                });

        JavaPairRDD<Integer, Sale> salesRDD = javaFunctions(sc)
                .cassandraTable("test", "sales", mapRowTo(Sale.class))
                .keyBy(new Function<Sale, Integer>() {
                    @Override
                    public Integer call(Sale sale) throws Exception {
                        return sale.getProduct();
                    }
                });
        List<Tuple2<Product, Optional<Sale>>> saleResults = productsRDD
                .leftOuterJoin(salesRDD).values().collect();

        for (Tuple2<Product, Optional<Sale>> saleResult : saleResults) {
            System.out.println(saleResult);
        }

        List<Tuple2<Product, Optional<Summary>>> results = productsRDD
                .leftOuterJoin(summariesRDD).values().collect();

        for (Tuple2<Product, Optional<Summary>> result : results) {
            System.out.println(result);
        }
    }

    public static class Product implements Serializable {

        private Integer id;
        private String name;
        private List<Integer> parents;

        public Product() {
        }

        public Product(Integer id, String name, List<Integer> parents) {
            this.id = id;
            this.name = name;
            this.parents = parents;
        }

        public Integer getId() {
            return id;
        }

        public void setId(Integer id) {
            this.id = id;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public List<Integer> getParents() {
            return parents;
        }

        public void setParents(List<Integer> parents) {
            this.parents = parents;
        }

        @Override
        public String toString() {
            return MessageFormat.format(
                    "Product'{'id={0}, name=''{1}'', parents={2}'}'", id, name,
                    parents);
        }
    }

    public static class Sale implements Serializable {

        private UUID id;
        private Integer product;
        private BigDecimal price;

        public Sale() {
        }

        public Sale(UUID id, Integer product, BigDecimal price) {
            this.id = id;
            this.product = product;
            this.price = price;
        }

        public UUID getId() {
            return id;
        }

        public void setId(UUID id) {
            this.id = id;
        }

        public Integer getProduct() {
            return product;
        }

        public void setProduct(Integer product) {
            this.product = product;
        }

        public BigDecimal getPrice() {
            return price;
        }

        public void setPrice(BigDecimal price) {
            this.price = price;
        }

        @Override
        public String toString() {
            return MessageFormat.format(
                    "Sale'{'id={0}, product={1}, price={2}'}'", id, product,
                    price);
        }
    }

    public static class Summary implements Serializable {

        private Integer product;
        private BigDecimal summary;

        public Summary() {
        }

        public Summary(Integer product, BigDecimal summary) {
            this.product = product;
            this.summary = summary;
        }

        public Integer getProduct() {
            return product;
        }

        public void setProduct(Integer product) {
            this.product = product;
        }

        public BigDecimal getSummary() {
            return summary;
        }

        public void setSummary(BigDecimal summary) {
            this.summary = summary;
        }

        @Override
        public String toString() {
            return MessageFormat.format(
                    "Summary'{'product={0}, summary={1}'}'", product, summary);
        }
    }
}
