FROM alpine:edge


RUN apk add cmake gcc g++ make openssh-server gdb rsync openssh-sftp-server findutils\
    util-linux pciutils usbutils coreutils binutils grep \
    && rm -rf /var/cache/apk/*

RUN mkdir /var/run/sshd
RUN echo 'root:root' | chpasswd
RUN sed -i 's/PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

ENV NOTVISIBLE "in users profile"
RUN echo "export VISIBLE=now" >> /etc/profile

RUN adduser -D -u 1000 debugger
RUN echo 'debugger:pwd' | chpasswd

########################################################
# Add custom packages and development environment here
#######################################################
COPY entrypoint.sh .
CMD sh entrypoint.sh

