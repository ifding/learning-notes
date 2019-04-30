import com.datastax.driver.core.*;
import com.datastax.driver.core.utils.Bytes;
import com.google.common.collect.ImmutableMap;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.util.Map;

/**
 * Created by Ken on 12/19/16.
 *
 * insert and retrieves values in BLOB columns.
 * the Java driver maps this type to java.nio.ByteBuffer.
 * create a new keyspace "examples" and a table "examples.blobs", insert data in the table.
 */

public class Blobs {

    static String[] CONTACT_POINTS = {"127.0.0.1"};
    static int PORT = 9042;

    static File FIFE = new File(Blobs.class.getResource("/cassandra_logo.png").getFile());

    public static void main(String[] args) throws IOException {
        Cluster cluster = null;
        try {
            cluster = Cluster.builder()
                    .addContactPoints(CONTACT_POINTS).withPort(PORT).build();
            Session session = cluster.connect();

            createSchema(session);
            insertBlobs(session);
            retrieveColumn(session);
            insertConcurrent(session);
            readAndWrite(session);
        } finally {
            if (cluster != null)
                cluster.close();
        }
    }

    private static void createSchema(Session session) {
        session.execute("CREATE KEYSPACE IF NOT EXISTS examples " +
                "WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1}");
        session.execute("CREATE TABLE IF NOT EXISTS examples.blobs(k int PRIMARY KEY, b blob, m map<text, blob>)");
    }

    private static void insertBlobs(Session session) {
        // one way to get a byte buffer is to allocate it and fill it.
        ByteBuffer buffer = ByteBuffer.allocate(16);
        while (buffer.hasRemaining())
            buffer.put((byte) 0xFF);

        // the driver expects a buffer that is ready for reading, considers all data between buffer.position()
        // and buffer.limit(). While finished writing, the position is at the end.
        assert buffer.limit() - buffer.position() == 0;

        buffer.flip();

        // now the position is back to the beginning, so the driver will see all 16 bytes.
        assert buffer.limit() - buffer.position() == 16;

        session.execute("INSERT INTO examples.blobs (k,b,m) VALUES (1, ?, ?)",
                buffer, ImmutableMap.of("test", buffer));
    }

    private static void retrieveColumn(Session session) {
        Row row = session.execute("SELECT b, m FROM examples.blobs WHERE k = 1").one();

        ByteBuffer buffer = row.getBytes("b");

        //the driver always returns buffers that are ready for reading
        assert buffer.limit() - buffer.position() == 16;

        // uses getters to read from buffer from position() to limit()
        for (int i = buffer.position(); i < buffer.limit(); i++) {
            byte b = buffer.get(i);
            assert b == (byte) 0xFF;
        }

        // another way is to use relative getters
        //while (buffer.hasRemaining()) {
        //    byte b = buffer.get();
        //    assert b == (byte) 0xFF;
        //}

        // note that relative getters change the position, when we're done reading it is at the end again.
        //assert  buffer.position() == buffer.limit();

        // reset the position for the next operation
        //buffer.flip();

        // Yet another way is to convert the buffer to a byte array
        //byte[] array = Bytes.getArray(buffer);
        //assert array.length == 16;
        //for (byte b : array) {
        //    assert b == (byte) 0xFF;
        //}

        // the map columns illustrates the pitfalls with position() and array().
        Map<String, ByteBuffer> m = row.getMap("m", String.class, ByteBuffer.class);
        ByteBuffer byteBuffer = m.get("test");

        // we did get back a buffer that contains 16 bytes as expected
        assert byteBuffer.limit() - byteBuffer.position() == 16;

        // it is not positioned at 0, its backing array contains more than 16 bytes.
        assert byteBuffer.position() == 16;
        assert byteBuffer.array().length == 32;
    }

    private static void insertConcurrent(Session session) {
        PreparedStatement preparedStatement = session.prepare("INSERT INTO examples.blobs (k,b) VALUES (1, :b)");

        // this is another convenient utility provided by the driver. it's useful for tests.
        ByteBuffer buffer = Bytes.fromHexString("0xffffff");

        // pass a byte buffer to a bound statement, it creates a shallow copy internally with the buffer.duplicate()
        BoundStatement boundStatement = preparedStatement.bind();
        boundStatement.setBytes("b", buffer);

        // this means you can now move in the original bufer, without affecting the insertion if it happens later.
        buffer.position(buffer.limit());

        session.execute(boundStatement);
        Row row = session.execute("SELECT b FROM examples.blobs WHERE k = 1").one();
        assert Bytes.toHexString(row.getBytes("b")).equals("0xffffff");

        buffer.flip();

        // duplicate() only performs a shallow copy. the two buffers still share the same contents
        buffer.put(0, (byte) 0xaa);
        session.execute(boundStatement);
        row = session.execute("SELECT b FROM examples.blobs WHERE k = 1").one();
        assert Bytes.toHexString(row.getBytes("b")).equals("0xaaffff");
    }

    private static void readAndWrite(Session session) throws IOException {
        ByteBuffer buffer = readAll(FIFE);
        session.execute("INSERT INTO examples.blobs (k,b) VALUES (1,?)", buffer);

        File tmpFile = File.createTempFile("blob", ".png");
        System.out.printf("Writing retrieved buffer to %s%n", tmpFile.getAbsoluteFile());

        Row row = session.execute("SELECT b FROM examples.blobs WHERE k = 1").one();
        writeAll(row.getBytes("b"),tmpFile);
    }

    // this reads the whole file in memory in one go.
    private static ByteBuffer readAll(File file) throws IOException {
        FileInputStream inputStream  = null;

        try {
            inputStream = new FileInputStream(file);
            FileChannel channel = inputStream.getChannel();
            ByteBuffer buffer = ByteBuffer.allocate((int) channel.size());
            channel.read(buffer);
            buffer.flip();
            return buffer;
        } finally {
            inputStream.close();
        }
    }

    private static void writeAll(ByteBuffer buffer, File file) throws IOException {
        FileOutputStream outputStream = null;

        try {
            outputStream = new FileOutputStream(file);
            FileChannel channel = outputStream.getChannel();
            channel.write(buffer);
        } finally {
            outputStream.close();
        }
    }
}
