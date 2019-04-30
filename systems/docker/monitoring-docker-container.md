In the software development, Monitor is a big feature we need to concen. It is not only for monitoring the production. But we also need to monitor your testing environment (e.g. for performance test, stress test).

In this chapter, we will cover the following topics about being able to monitor our Docker infrastructure and applications at scale:
- The importance of monitoring
- Collecting monitored data in Graphite
- Monitoring Docker with collectd
- Consolidating logs in an ELK stack
- Sending logs from Docker

## The importance of monitoring

Monitoring is important as it provides a source of feedback on the Docker on the Docker deployment that we built. It answers several questions about our application from low-level operating system performance to high-level business targets. Having proper instrumentation inserted in our Docker hosts allows us to identify our system’s state. We can use this source of feedback to identify whether our application is behaving as originally planned.

If our initial hypothesis was incorrect, we can use the feedback data to revise our plan and change our system accordingly by tuning our Docker host and containers or updating our running Docker application. We can also use the same monitoring process to identify errors and bugs after our system is deployed to production.

Docker has built-in features to log and monitor. By default, a Docker host stores a Docker container’s standard output and error streams to JSON files in
``` 
/var/lib/docker/<container_id>/<container_id>-json.log
```
 
The docker logs command asks the Docker engine daemon to read the content of the files here.

Another monitoring facility is the docker stats command. This queries the Docker engine’s remote API’s `/containers/<container_id>/stats` endpoint to report runtime statistics about the running container’s control group regarding its CPU, memory, and network usage.

The built-in docker logs and docker stats commands work well to monitor our Docker applications for development and small-scale deployments. When we get to a point in our production-grade Docker deployment where we manage tens, hundreds, or even thousands of Docker hosts, this approach will no longer be scalable. It is not feasible to log in to each of our thousand Docker hosts and type docker  logs and docker stats. Doing this one by one also makes it difficult to create a more holistic picture of our entire Docker deployment. 

Hence, it is important to be able to consolidate all of the events and metrics from our Docker deployments into a centralized monitoring infrastructure. It allows our operations to scale by having a single point to ask what is happening to our system. 

## Collecting metrics to Graphite

To begin monitoring our Docker deployments, we must first set up an endpoint to send our monitored values to. Graphite is a popular stack for collecting various metrics. Its plaintext protocol is very popular because of its simplicity. Another feature of graphite is that it can render the data it gathers into graphs. We can then consolidate these graphs to build a dashboard. The dashboard we crafted in the end will show the various kinds of information that we need to monitor our Docker application.

In this section, we will set up the following components of Graphite to create a minimal stack:
- carbon-cache: This is the Graphite component that receives metrics over the network. This implements the simple plaintext protocol, which is a more advanced but smaller and optimized format to receive metrics.
- whisper: This is a file-based bounded time series database in which the carbon-cache persists the metrics it receives. Its bounded or fixed-size nature makes it an ideal solution for monitoring. However, in practice we are mostly interested in monitoring our application until a fixed point.
- graphite-web: This reads the whisper database to render graphs and dashboards. It is also optimized to create such visualizations in real time by querying carbon-cache endpoints to display data that is yet to be persistent in the whisper database as well.

[https://github.com/graphite-project](https://github.com/graphite-project)

## Graphite in production

In production, this simple Graphite setup will reach its limits as we monitor more and more metrics in our Docker deployment. We need to scale this out in order to keep up with the increased number of metrics that we monitor. With this, you need to deploy Graphite with a cluster setup.

To scale out the metric processing capacity of carbon-cache, we need to augment it with a carbon-relay and carbon-aggregator. For graphite-web to the more responsive, we need to scale it out horizontally along with other caching components, such as memcached. We also need to add another graphite-web instance that connects to other graphite-web instances to create a unified view of all the metrics. The whisper databases will be co-located with a carbon-cache and graphite-web, so it will scale-out naturally along with them.


### Monitoring with collectd

In this section, we will use collectd, a popular system statistics collection daemon. It is very lightweight and high-performance C program. This makes it a noninvasive monitoring software because it doesn’t consume many resources from the system it monitors. Being lightweight, it is very simple to deploy as it requires minimum dependencies. It has a wide variety of plugins to monitor almost every component of our system.

### Consolidating logs in an ELK stack

Not all statuses of our Docker hosts and containers are readily available to be queried with our monitoring solution in collectd and Graphite. Some events and metrics are only available as raw lines of text in log files. We need to transform these raw and unstructured logs to meaningful metrics. Similar to raw metrics, we can later ask higher-level questions on what is happening in our Docker-based application through analytics.

The ELK stack is a popular combination suite from Elastic that addresses these problems. Each letter in the acronym represents each of its components. The following is a description of each of them:

- Logstash: Logstash is the component that is used to collect and manage logs and events. It is the central point that we use to collect all the logs from different log sources, such as multiple Docker hosts and containers running in our deployment. We can also use Logstash to transform and annotate the logs we receive. This allows us to search and explore the richer features of our logs later.

- Elasticsearch: Elasticsearch is a distributed search engine that is highly scalable. Its sharding capabilities allow us to grow and scale our log storage as we continue to receive more and more logs from our Docker containers. Its database engine is document-oriented. This allows us to store and annotate logs as we see fit as we continue to discover more insights about the events we are managing in our large Docker deployments.

- Kibana: Kibana is an analytics and search dashboard for Elasticsearch. Its simplicity allows us to create dashboards for our Docker applications. However, Kibana is also very flexible to customize, so we can build dashboards that can provide valuable insights to people who want to understand our Docker-based applications, whether it is a low-level technical detail or higher-level business need.









