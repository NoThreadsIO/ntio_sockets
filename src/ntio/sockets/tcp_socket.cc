// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "tcp_socket.h"

#include "ntio/core/logger.h"
#include "ntio/sockets/internal/socket_utils.h"

namespace ntio::sockets {

using internal::CloseTcpSocket;
using internal::OpenTcpSocket;

/*
 * TcpSocket
 */

const TcpSocket::Settings TcpSocket::kDefaultSettings = {
    .connection_timeout = std::chrono::seconds(1),
    .read_timeout = std::chrono::seconds(1),
    .write_timeout = std::chrono::seconds(1),
};

TcpSocket::TcpSocket() : TcpSocket(kDefaultSettings) {}

TcpSocket::TcpSocket(Settings settings) : TcpSocket(OpenTcpSocket(), settings) {}

TcpSocket::TcpSocket(int fd, TcpSocket::Settings settings) : fd_(fd), settings_(settings) {
  LOG_INFO("Opened socket fd=" + std::to_string(fd));
}

TcpSocket::~TcpSocket() {
  LOG_INFO("Close socket fd=" + std::to_string(fd_));
  CloseTcpSocket(fd_);
}

AsyncConnect TcpSocket::Connect(EndPoint end_point) const noexcept {
  return AsyncConnect(fd_, end_point, settings_.connection_timeout);
}

AsyncRead TcpSocket::Read(std::vector<uint8_t>* data, size_t len) const noexcept {
  return AsyncRead(fd_, settings_.read_timeout, data, len);
}

AsyncWrite TcpSocket::Write(const std::vector<uint8_t>& data, size_t len) const noexcept {
  return AsyncWrite(fd_, settings_.write_timeout, data, len);
}

}  // namespace ntio::sockets
