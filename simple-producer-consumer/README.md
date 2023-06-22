# Example: librdkafka C++ Producer Consumer

This is a producer consumer example utilizing the librdkafka library. 

The implementation uses Docker and docker-compose to deploy the application.

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

Start: `docker-compose up -d`

Stop: `docker-compose down`
