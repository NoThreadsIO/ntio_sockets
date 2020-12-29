FROM alpine:3.12
RUN apk update \
    && apk --no-cache add mosquitto

EXPOSE 1883

CMD /usr/sbin/mosquitto -c /etc/mosquitto/mosquitto.conf
