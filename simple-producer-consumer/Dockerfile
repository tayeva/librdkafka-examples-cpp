FROM tayeva/librdkafka:2.1.1

WORKDIR /app

COPY . .

RUN mkdir build \
    && cd build \
    && cmake .. \
    && cmake --build .
