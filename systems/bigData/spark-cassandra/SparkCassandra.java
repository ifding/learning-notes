package example;

import com.datastax.driver.core.Session;
import com.datastax.spark.connector.cql.CassandraConnector;
import com.datastax.spark.connector.japi.CassandraRow;
import org.apache.hadoop.util.StringUtils;
import org.apache.spark.SparkConf;
import org.apache.spark.api.java.JavaRDD;
import org.apache.spark.api.java.JavaSparkContext;
import org.apache.spark.api.java.function.Function;
import org.apache.spark.api.java.function.PairFunction;
import scala.Tuple2;

import java.io.Serializable;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

import static com.datastax.spark.connector.japi.CassandraJavaUtil.javaFunctions;
import static com.datastax.spark.connector.japi.CassandraJavaUtil.mapRowTo;
import static com.datastax.spark.connector.japi.CassandraJavaUtil.mapToRow;

/**
 * Created by Ken on 12/20/16.
 * How to use Spark Cassandra Connector with Java.
 */

public class SparkCassandra {

    public static void main(String[] args) {

        SparkConf conf = new SparkConf();
        conf.setAppName("SparkCassandra");
        conf.setMaster("local[2]");
        conf.set("spark.cassandra.connection.host", "localhost");

        JavaSparkContext sc = new JavaSparkContext(conf);

        createData(sc);
        calculate(sc);
        showData(sc);

        sc.stop();
    }

    private static void createData(JavaSparkContext sc) {

        CassandraConnector connector = CassandraConnector.apply(sc.getConf());

        try (Session session = connector.openSession()){
            session.execute("DROP KEYSPACE IF EXISTS test");
            session.execute("CREATE KEYSPACE test WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1}");
            session.execute("CREATE TABLE test.product (id INT PRIMARY KEY, name TEXT)");
            session.execute("CREATE TABLE test.people (id INT, name TEXT, birth_date TIMESTAMP, PRIMARY KEY (id))");
        }

        List<Product> products = Arrays.<Product>asList(new Product(1, "apple"), new Product(2, "banana"),
                new Product(3, "pineapple"), new Product(4, "peach"), new Product(5, "banana"),
                new Product(6, "apple"));

        JavaRDD<Product> productJavaRDD = sc.parallelize(products);

        javaFunctions(productJavaRDD).writerBuilder("test","product", mapToRow(Product.class)).saveToCassandra();

        List<Person> people = Arrays.asList(
                Person.newInstance(1, "John", new Date()),
                Person.newInstance(2, "Anna", new Date()),
                Person.newInstance(3, "Andrew", new Date()),
                Person.newInstance(4, "Anna", new Date())
        );
        JavaRDD<Person> personJavaRDD = sc.parallelize(people);

        javaFunctions(personJavaRDD).writerBuilder("test", "people", mapToRow(Person.class)).saveToCassandra();
    }

    private static void calculate(JavaSparkContext sc) {

        javaFunctions(sc).cassandraTable("test","product", mapRowTo(Product.class))
                .mapToPair(new PairFunction<Product, Product, Integer>() {
                @Override
                public Tuple2<Product, Integer> call (Product product) throws Exception {
                    return new Tuple2<Product, Integer>(product, 1);
                }
        }).reduceByKey((integer, integer2) -> integer + integer2)
                .foreach(productCounter -> System.out.println(productCounter._1().getName() + ": " + productCounter._2()));
    }

    private static void showData(JavaSparkContext sc) {

        // use case: read the data as an RDD of CassandraRows and convert them to strings
        JavaRDD<String> cassandraRowsRDD = javaFunctions(sc)
                .cassandraTable("test","people")
                .map(new Function<CassandraRow, String>() {
                    @Override
                    public String call(CassandraRow cassandraRow) throws Exception {
                        return cassandraRow.toString();
                    }
                });
        System.out.println("Data as CassandraRows: \n" + StringUtils.join("\n", cassandraRowsRDD.collect()));

        // use case: read the data as an RDD of Person and convert them to strings
        JavaRDD<String> personRDD = javaFunctions(sc)
                .cassandraTable("test", "people", mapRowTo(Person.class))
                .map(new Function<Person, String>() {
                    @Override
                    public String call(Person person) throws Exception {
                        return person.toString();
                    }
                });
        System.out.println("Data as Person: \n" + StringUtils.join("\n", personRDD.collect()));

        // use case: filter rows on the database with use of the where clause
        JavaRDD<String> peopleRDD = javaFunctions(sc)
                .cassandraTable("test", "people", mapRowTo(Person.class))
                .where("name=?", "Anna")
                .map(new Function<Person, String>() {
                    @Override
                    public String call(Person person) throws Exception {
                        return person.toString();
                    }
                });
        System.out.println("Data filtered by (name= 'Anna'): \n" + StringUtils.join("\n", peopleRDD.collect()));
    }

    public static class Product implements Serializable {
        private long id;
        private String name;

        public Product(long id, String name) {
            this.id = id;
            this.name = name;
        }

        public long getId() {
            return id;
        }

        public String getName() {
            return name;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (o == null || getClass() != o.getClass()) return false;

            Product product = (Product) o;

            return name.equals(product.name);
        }

        @Override
        public int hashCode() {
            return name.hashCode();
        }
    }

    public static class Person implements Serializable {
        private Integer id;
        private String name;
        private Date birthDate;

        public static Person newInstance(Integer id, String name, Date birthDate) {
            Person person = new Person();
            person.setId(id);
            person.setName(name);
            person.setBirthDate(birthDate);
            return person;
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

        public Date getBirthDate() {
            return birthDate;
        }

        public void setBirthDate(Date birthDate) {
            this.birthDate = birthDate;
        }

        @Override
        public String toString() {
            return com.google.common.base.Objects.toStringHelper(this)
                    .add("id", id)
                    .add("name", name)
                    .add("birthDate", birthDate)
                    .toString();
        }
    }
}
