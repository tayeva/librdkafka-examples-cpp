cmake_minimum_required(VERSION 3.12)
project(demo)

set(CMAKE_CXX_STANDARD 17)

add_executable(producer ${PROJECT_SOURCE_DIR}/src/producer.cpp)
add_executable(consumer ${PROJECT_SOURCE_DIR}/src/consumer.cpp)

target_link_libraries(producer PUBLIC rdkafka rdkafka++)
target_link_libraries(consumer PUBLIC rdkafka rdkafka++)