FROM alpine:3.13

WORKDIR /usr/src/ntio_sockets
RUN apk --no-cache add cmake gcc g++ make
COPY . .

WORKDIR /usr/src/ntio_sockets/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DNTIO_SOCKETS_LOG_LEVEL="" \
    -DNTIO_SOCKETS_ENABLE_EXAMPLES=ON -DNTIO_SOCKETS_ENABLE_TESTS=ON ..
RUN make -j