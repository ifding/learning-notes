import com.datastax.driver.core.*;

/**
 * Created by Ken on 12/19/16.
 *
 * connects to a Cassandra cluster and extracts basic information from it.
 * gathers information about a Cassandra cluster's topology (which node belong to the cluster)
 * and schema (what keyspaces, tables, etc. exist in this cluster).
 */

public class ReadVersion {

    static String[] CONTACT_POINTS = {"127.0.0.1"};
    static int PORT = 9042;

    public static void main(String[] args) {

        Cluster cluster = null;

        try {

            // creates a single instance and share it throughout the application
            cluster = Cluster.builder()
                    .addContactPoints(CONTACT_POINTS).withPort(PORT)
                    .build();

            // the seession is what you use to execute queries, it should be reused.
            Session session = cluster.connect();

            // uses execute to send a query to Cassandra, returns a ResultSet, a collection of Row objects.
            ResultSet resultSet = session.execute("select release_version from system.local");

            // extract the first row, the only one in this case
            Row row = resultSet.one();

            String releaseVersion = row.getString("release_version");
            System.out.printf("Cassandra version is: %s%n", releaseVersion);

            Metadata metadata = cluster.getMetadata();
            System.out.printf("Connected to cluster: %s%n", metadata.getClusterName());

            for (Host host : metadata.getAllHosts()) {
                System.out.printf("Datacenter: %s; Host: %s; Rack: %s%n",
                        host.getDatacenter(), host.getAddress(), host.getRack());
            }

            for (KeyspaceMetadata keyspace : metadata.getKeyspaces()) {
                for (TableMetadata table : keyspace.getTables()) {
                    System.out.printf("Keyspace: %s; Table: %s%n",
                            keyspace.getName(), table.getName());
                }
            }
        } finally {
            // close the cluster and any session that was created from this cluster.
            // it frees underlying resources, TCP connections, thread pools and so on.
            if (cluster != null)
                cluster.close();
        }
    }
}