#include <chrono>
#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "librdkafka/rdkafkacpp.h"

static volatile sig_atomic_t run = 1;

static void sigterm(int sig) { run = 0; }

class DeliveryCallback : public RdKafka::DeliveryReportCb {
public:
  void dr_cb(RdKafka::Message &message) {
    if (message.err()) {
      std::cerr << "Message delivery failed: " << message.errstr() << std::endl;
    } else {
      std::cout << "Message delivered to topic " << message.topic_name() << " ["
                << message.partition() << "] at offset " << message.offset()
                << std::endl;
    }
  }
};

int main() {
  std::string brokers = "kafka:9092";
  std::string topic = "hello-world";
  int n_messages = 100;
  RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
  std::string errstr;
  if (conf->set("bootstrap.servers", brokers, errstr) !=
      RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(EXIT_FAILURE);
  }

  signal(SIGINT, sigterm);
  signal(SIGTERM, sigterm);

  DeliveryCallback dr_cb;
  if (conf->set("dr_cb", &dr_cb, errstr) != RdKafka::Conf::CONF_OK) {
    std::cerr << errstr << std::endl;
    exit(EXIT_FAILURE);
  }
  RdKafka::Producer *producer = RdKafka::Producer::create(conf, errstr);
  if (!producer) {
    std::cerr << "Failed to create producer: " << errstr << std::endl;
    exit(EXIT_FAILURE);
  }
  delete conf;

  for (int i = 0; run && i <= 100; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::ostringstream oss;
    oss << "hello world " << i;
    ;
    const std::string message = oss.str();
  retry:
    RdKafka::ErrorCode err = producer->produce(
        topic, RdKafka::Topic::PARTITION_UA, RdKafka::Producer::RK_MSG_COPY,
        const_cast<char *>(message.c_str()), message.size(), NULL, 0, 0, NULL,
        NULL);
    if (err != RdKafka::ERR_NO_ERROR) {
      std::cerr << "Failed to produce to topic " << topic << ": "
                << RdKafka::err2str(err) << std::endl;
      if (err == RdKafka::ERR__QUEUE_FULL) {
        producer->poll(1000);
        goto retry;
      }
    } else {
      std::cout << "Enqueued message: " << message << " (" << message.size()
                << " bytes) "
                << "for topic " << topic << std::endl;
    }
    producer->poll(0);
  }
  std::cout << "Flushing final messages..." << std::endl;
  producer->flush(10 * 1000);
  if (producer->outq_len() > 0) {
    std::cerr << producer->outq_len() << " message(s) were not delivered"
              << std::endl;
  }
  delete producer;
  return EXIT_SUCCESS;
}