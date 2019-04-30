
import org.apache.flink.api.java.utils.ParameterTool;
import org.apache.flink.streaming.api.datastream.DataStream;
import org.apache.flink.streaming.api.environment.StreamExecutionEnvironment;
import org.apache.flink.streaming.api.functions.source.SourceFunction;
import org.apache.flink.streaming.connectors.kafka.FlinkKafkaProducer08;
import org.apache.flink.streaming.util.serialization.SimpleStringSchema;

import java.util.Properties;

/**
 * Created by Ken on 12/17/16.
 *
 * Simple example for writing data into Kafka
 *
 * The following arguments are required:
 * - "bootstrap.servers" (comma separated list of kafka brokers)
 * - "topic" the name of the topic to write data to.
 *
 * This is an example command lint argument:
 *     --topic test --bootstrap.servers localhost:9092
 */

public class WriteIntoKafka {

    public static void main(String[] args) throws Exception {

        // create execution environment
        StreamExecutionEnvironment env = StreamExecutionEnvironment.getExecutionEnvironment();

        // parse user parameters
        ParameterTool parameterTool = ParameterTool.fromArgs(args);

    //    Properties properties = new Properties();
    //    properties.setProperty("bootstrap.servers","localhost:9092");
    //    DataStream<String> messageStream = env.addSource(new SimpleStringGenerator());
    //    messageStream.addSink(new FlinkKafkaProducer08<String>("topic", new SimpleStringSchema(),properties));

        // add a simple source which is writing some strings
        DataStream<String> messageStream = env.addSource(new SimpleStringGenerator());

        // write stream to kafka
        messageStream.addSink(new FlinkKafkaProducer08<String>(
                parameterTool.getRequired("bootstrap.servers"),
                parameterTool.getRequired("topic"),
                new SimpleStringSchema()));

        env.execute();
    }


    // the producer generates messages using this class and send the string to the topic
    public static class SimpleStringGenerator implements SourceFunction<String> {

        private static final long serialVersionUID = 2174904787118597072L;
        boolean running = true;
        long i = 0;
        @Override
        public void run(SourceContext<String> ctx) throws Exception {
            while (running) {
                ctx.collect("element-" + (i++));
                Thread.sleep(10);
            }
        }

        @Override
        public void cancel() {
            running = false;
        }
    }
}
