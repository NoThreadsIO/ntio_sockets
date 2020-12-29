FROM alpine:edge

WORKDIR /usr/src/ntio_sockets
RUN apk --no-cache add cmake gcc g++ make
COPY . .

WORKDIR /usr/src/ntio_sockets/build
RUN cmake -DCMAKE_BUILD_TYPE=Release -DLOG_LEVEL="" ..
RUN make -j