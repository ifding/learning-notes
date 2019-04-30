import org.apache.flink.api.common.functions.MapFunction;
import org.apache.flink.api.java.utils.ParameterTool;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.connectors.kafka.FlinkKafkaConsumer08;
import org.apache.flink.streaming.util.serialization.SimpleStringSchema;

import java.util.Properties;

/**
 * Created by Ken on 12/17/16.
 *
 * Simple example on how to read with a Kafka consumer
 *
 * Note that the Kafka source is expecting the following parameters to be set
 *  - "bootstrap.servers" (comma separated list of kafka brokers)
 *  - "zookeeper.connect" (comma separated list of zookeeper servers)
 *  - "group.id" the id of the consumer group
 *  - "topic" the name of the topic to read data from.
 *
 *  Pass these parameters using
 *      "--bootstrap.servers host:port,host1:port1 --zookeeper.connect host:port --topic testTopic"
 *
 *  In this example, It is valid like:
 *      --topic test --bootstrap.servers localhost:9092 --zookeeper.connect localhost:2181 --group.id myGroup
 */

public class ReadFromKafka {

    public static void main(String[] args) throws Exception {

        // create execution environment
        StreamExecutionEnvironment env = StreamExecutionEnvironment.createLocalEnvironment();

        // parse user parameters
        ParameterTool parameterTool = ParameterTool.fromArgs(args);

        DataStream<String> messageStream = env.addSource(
                new FlinkKafkaConsumer08<String>(parameterTool.getRequired("topic"),
                        new SimpleStringSchema(), parameterTool.getProperties()));

        //Properties properties = new Properties();
        //properties.setProperty("bootstrap.servers","localhost:9092");
        //properties.setProperty("group.id","myGroup");
        //DataStream<String> messageStream = env.addSource(
        //        new FlinkKafkaConsumer08<String>("topic",new SimpleStringSchema(),properties));

        // print() will write the contents of the stream to the TaskManager's standard out stream
        // the rebalance call is causing a repartitioning of the data so that all machines see the messages
        messageStream.rebalance().map(new MapFunction<String, String>() {
            private static final long serialVersionUID = -6867736771747690202L;

            @Override
            public String map(String value) throws Exception {
                return "Kafka and Flink say: " + value;
            }
        }).print();

        env.execute();
    }
}