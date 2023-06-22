# Example: librdkafka C++ Producer Consumer

This is a producer consumer example utilizing the librdkafka library. 

The implementation uses Docker and docker-compose to deploy the application. It runs single, standalone instances of kafka and zookeeper. The demonstrations uses one producer, one consumer, and one topic.

## Architecture

- Client library: [librdkafka](https://github.com/confluentinc/librdkafka)
- Client library Docker image: [tayeva/librdkafka](https://hub.docker.com/r/tayeva/librdkafka)
- Kafka Docker image: [confluentinc/cp-kafka](https://hub.docker.com/r/confluentinc/cp-kafka)
- Zookeeper Docker image: [confluentinc/cp-zookeeper](https://hub.docker.com/r/confluentinc/cp-zookeeper)
- [Docker engine](https://docs.docker.com/engine/)
- [Docker compose](https://docs.docker.com/compose/)

### Architecture diagram:

![Architecture diagram](https://github.com/tayeva/librdkafka-examples-cpp/blob/main/simple-producer-consumer/architecture.png)

## Running

First start the core services: kafka and zookeeper services.

(from within this directory)

`docker-compose -f docker-compose-core.yml up`

Open up another terminal and start the producer consumer services:

`docker-compose -f docker-compose-pc.yml up`

By starting both of these services in two separate terminals in this manner you should see the aggregated logs for both sets of services. Once the producer and consumer processes are done you can hit CTRL+C to stop them individually or open a third terminal and run the down commands. If you use CTRL+C to stop the commands you should still clean up the resources with the down commands.

`docker-compose -f docker-compose-core.yml down`

`docker-compose -f docker-compose-pc.yml down`
