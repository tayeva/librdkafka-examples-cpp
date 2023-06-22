#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "librdkafka/rdkafkacpp.h"

static volatile sig_atomic_t run = 1;

static void sigterm(int sig) { run = 0; }

int main() {
  std::string brokers = "kafka:9092";
  std::vector<std::string> topics = {"hello-world"};
  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  std::string errstr;

  if (conf->set("bootstrap.servers", brokers, errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << "Failed to set brokers: " << errstr << std::endl;
    exit(EXIT_FAILURE);
  }
  if (conf->set("group.id", "demo-group", errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << "Failed to set group id: " << errstr << std::endl;
    exit(EXIT_FAILURE);
  }
  if (conf->set("auto.offset.reset", "earliest", errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << "Failed to set auto.offset.reset: " << errstr << std::endl;
    exit(EXIT_FAILURE);
  }

  signal(SIGINT, sigterm);
  signal(SIGTERM, sigterm);

  RdKafka::KafkaConsumer *consumer =
      RdKafka::KafkaConsumer::create(conf, errstr);
  if (!consumer) {
    std::cerr << "Failed to create consumer: " << errstr << std::endl;
    exit(EXIT_FAILURE);
  }
  delete conf;

  RdKafka::Topic *topic =
      RdKafka::Topic::create(consumer, topics[0], NULL, errstr);
  if (!topic) {
    std::cerr << "Failed to create topic: " << errstr << std::endl;
    exit(EXIT_FAILURE);
  }

  RdKafka::ErrorCode err = consumer->subscribe(topics);
  if (err != RdKafka::ERR_NO_ERROR) {
    std::cerr << "Failed to subscribe to topic: " << RdKafka::err2str(err)
              << std::endl;
    exit(EXIT_FAILURE);
  }

  long message_count = 0;
  int64_t message_bytes = 0;
  while (run) {
    RdKafka::Message *message = consumer->consume(100);
    switch (message->err()) {
    case RdKafka::ERR_NO_ERROR:
      std::cout << "Received message: "
                << std::string(static_cast<char *>(message->payload()),
                               message->len())
                << std::endl;
      message_count++;
      message_bytes += message->len();
      break;
    case RdKafka::ERR__TIMED_OUT:
      break;
    default:
      std::cerr << "Error while consuming message: "
                << RdKafka::err2str(message->err()) << std::endl;
      break;
    }
    delete message;
    if (message_count >= 100)
      break;
  }

  consumer->close();

  delete consumer;

  std::cout << "Consumed " << message_count << " messages (" << message_bytes
            << " bytes)" << std::endl;

  return EXIT_SUCCESS;
}