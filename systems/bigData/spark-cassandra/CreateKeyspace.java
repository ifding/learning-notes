import com.datastax.driver.core.Cluster;
import com.datastax.driver.core.ResultSet;
import com.datastax.driver.core.Row;
import com.datastax.driver.core.Session;

/**
 * Created by Ken on 12/19/16.
 *
 * create a keyspace "simplex" in the cluster, if it already exists, it will be reused.
 * create two tables "simplex.songs" and "simplex.playlists", if they exist already, they will be reused.
 * inserts a row in each table.
 */

public class CreateKeyspace {

    static String[] CONTACT_POINTS = {"127.0.0.1"};
    static int PORT = 9042;

    public static void main(String[] args) {

        CreateKeyspace client = new CreateKeyspace();

        try {
            client.connect(CONTACT_POINTS, PORT);
            client.createSchema();
            client.loadData();
            client.querySchema();
        } finally {
            client.close();
        }
    }

    private Cluster cluster;

    private Session session;

    // initiates a connecion to the cluster by the given contact point and port.
    public void connect(String[] contactPoints, int port) {

        cluster = Cluster.builder()
                .addContactPoints(contactPoints).withPort(port).build();

        System.out.printf("Connected to cluster:%s%n", cluster.getMetadata().getClusterName());

        session = cluster.connect();
    }

    // creates the keyspace and tables
    public void createSchema() {

        session.execute("CREATE KEYSPACE IF NOT EXISTS simplex WITH replication " +
                "= {'class': 'SimpleStrategy', 'replication_factor':1};");

        session.execute(
                "CREATE TABLE IF NOT EXISTS simplex.songs (" +
                        "id uuid PRIMARY KEY," +
                        "title text," +
                        "album text," +
                        "artist text," +
                        "tags set<text>," +
                        "data blob" +
                        ");");

        session.execute(
                "CREATE TABLE IF NOT EXISTS simplex.playlists (" +
                        "id uuid," +
                        "title text," +
                        "album text, " +
                        "artist text," +
                        "song_id uuid," +
                        "PRIMARY KEY (id, title, album, artist)" +
                        ");");
    }

    // inserts data into the tables
    public void loadData() {

        session.execute(
                "INSERT INTO simplex.songs (id, title, album, artist, tags) " +
                        "VALUES (" +
                        "756716f7-2e54-4715-9f00-91dcbea6cf50," +
                        "'La Petite Tonkinoise'," +
                        "'Bye Bye Blackbird'," +
                        "'Joséphine Baker'," +
                        "{'jazz', '2013'})" +
                        ";");

        session.execute(
                "INSERT INTO simplex.playlists (id, song_id, title, album, artist) " +
                        "VALUES (" +
                        "2cc9ccb7-6221-4ccb-8387-f22b6a1b354d," +
                        "756716f7-2e54-4715-9f00-91dcbea6cf50," +
                        "'La Petite Tonkinoise'," +
                        "'Bye Bye Blackbird'," +
                        "'Joséphine Baker'" +
                        ");");
    }

    // queries and displays data
    public void querySchema() {

        ResultSet resultSet = session.execute(
                "SELECT * FROM simplex.playlists " +
                        "WHERE id = 2cc9ccb7-6221-4ccb-8387-f22b6a1b354d;");

        System.out.printf("%-30s\t%-20s\t%-20s%n", "title", "album", "artist");
        for (Row row : resultSet) {
            System.out.printf("%-30s\t%-20s\t%-20s%n",
                    row.getString("title"),
                    row.getString("album"),
                    row.getString("artist"));
        }
    }

    // closes the session and the cluster
    public void close() {
        session.close();
        cluster.close();
    }
}
