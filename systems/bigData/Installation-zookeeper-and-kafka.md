Installation Zookeeper and Kafka server
=========
>02-06-2017

### Install On Mac os

* Install Zookeeper
```
brew install zookeeper
zkserver start
```

* Install Kalfa

 ```
 brew install kafka
 cd /usr/local/Cellar/kafka/0.10.0.1/libexec
 kafka-server-start config/server.properties  
 ```
 
### Install Zookeeper and Kalfa on Ubuntu

* Install Java
To add the repository
```
sudo add-apt-repository -y ppa:webupd8team/java
sudo apt-get update
sudo apt-get install oracle-java8-installer -y
sudo java -version
```
* Install Zookeeper
```
sudo apt-get install zookeeperd
netstat -ant | grep :2181
tcp6       0      0 :::2181                 :::*                    LISTEN
```
* Install Kafka
wget to download kafka
```
wget http://apache.claz.org/kafka/0.10.1.0/kafka_2.11-0.10.1.0.tgz
sudo tar -zxf kafka_2.11-0.10.1.0.tgz -C /usr/local
cd /usr/local
sudo mv kafka_2.11-0.10.1.0/ ./kafka
sudo  /usr/local/kafka/bin/kafka-server-start.sh /usr/local/kafka/config/server.properties
```

### Test Kafka

* Create a topic

 ```
 kafka-topics --create --zookeeper localhost:2181 --replication-factor 1 --partitions 1 --topic test
 kafka-topics --list --zookeeper localhost:2181 
 ```
* Create a producer

 ```
 kafka-console-producer --broker-list localhost:9092 --topic test
 hello world!
 ```
* Create a consumer in different terminal

 ```
 kafka-console-consumer --zookeeper localhost:2181 --topic test --from-beginning 
 ```
 
 ### Reference
 * [Install and Configure Apache Kafka on Ubuntu 16.04](https://devops.profitbricks.com/tutorials/install-and-configure-apache-kafka-on-ubuntu-1604-1/)
