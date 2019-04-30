Container and VM monitoring (ie, CPU, memory, and I/O loads)
=========
>02-06-2017

collectd + kafka

### Step 1: Building librdkafka
librdkafka is a C library implementation of the Apache Kafka protocol, containing both Producer and Consumer support. 

Set Up the Environment
* build-essential package contains an informational list of packages which are considered essential for building Ubuntu packages including gcc compiler, make and other required tools.

```
$ sudo apt-get update
$ sudo apt-get upgrade
$ sudo apt-get install build-essential
$ sudo apt-get install libpthread-stubs0-dev
```

Install librdkafka-dev on Ubuntu 16.04
* version: librdkafka-dev_0.8.6-1.1_amd64.deb
* description:	library implementing the Apache Kafka protocol (development headers)

```
$ sudo apt-get update
$ sudo apt-get install librdkafka-dev
```

(optional)building librdkafka

```
git clone https://github.com/edenhill/librdkafka.git
cd librdkafka/
./configure
make
sudo make install
```
(optional)usage in code

Link your program with ```-lrdkafka -lz -lpthread -lrt```.

public API

* The C API is documented in src/rdkafka.h
* The C++ API is documented in src-cpp/rdkafkacpp.h


### Step 2: Download and install collectd

```
wget https://storage.googleapis.com/collectd-tarballs/collectd-5.7.1.tar.bz2
tar xf collectd-version.tar.bz2
cd collectd-version
./configure
make all install
```

### Step 3: Start collectd
```
 nano /opt/collectd/etc/collectd.conf
```
Edit ```collectd.conf``` file and enable these plugins to collect system data and push it to kafka
```
LoadPlugin cpu
LoadPlugin df
LoadPlugin interface
LoadPlugin load
LoadPlugin memory
LoadPlugin ping
LoadPlugin write_kafka
```
Configure ```Ping```by uncommenting required lines like Ping Plugin:
```
<Plugin ping>
       Host "192.168.3.11"
       Interval 1.0
       Timeout 0.9
       TTL 255
       MaxMissed -1
</Plugin>
```
This plugin will monitor ping latency, packet loss and deviation to IP 192.168.3.11, sending ICMP echo very second.

Configure ```write_kafka```, change the localhost to the IP of kafka consumer
```
<Plugin write_kafka>
  Property "metadata.broker.list" "localhost:9092"
  <Topic "collectd">
    Format JSON
  </Topic>
</Plugin>
```

Start the collectd service

```
/opt/collectd/sbin/collectd -C /opt/collectd/etc/collectd.conf
```

### Step 4: Start Kafka cosumer

```
/usr/local/kafka/bin/kafka-console-consumer.sh --zookeeper localhost:2181 --topic collectd --from-beginning
```

### Step 5: (optional) apt-get install collectd

Install on Ubuntu/Debian, check the version
```
apt-get install collectd
```
Start and stop service
```
service collectd start
service collectd status
service collectd stop
service collectd restart
```

### Reference

* [The Apache Kafka C/C++ library](https://github.com/edenhill/librdkafka)
* [collectd – The system statistics collection daemon](https://collectd.org/)
* [How to Install and Configure ‘Collectd’ and ‘Collectd-Web’ to Monitor Server Resources in Linux](http://www.tecmint.com/install-collectd-and-collectd-web-to-monitor-server-resources-in-linux/)
* [How to set up monitoring stack using CollectD, Graphite, Grafana and Seyren on Ubuntu 14.04](https://community.rackspace.com/products/f/25/t/6800)
