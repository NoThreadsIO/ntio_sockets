// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "tcp_socket.h"

#include "ntio/sockets/internal/logger.h"
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

/*
 * TcpListener
 */

const TcpListener::Settings TcpListener::kDefaultSettings = {
    .back_log = 10,
    .accept_timeout = std::chrono::milliseconds(1000),
};

TcpListener::TcpListener() : TcpListener(kDefaultSettings) {}

TcpListener::TcpListener(TcpListener::Settings settings) : fd_(OpenTcpSocket()), settings_(settings) {
  LOG_DEBUG("Opened socket listen_fd=" + std::to_string(fd_));
}

TcpListener::~TcpListener() {
  LOG_DEBUG("Close socket listen_fd=" + std::to_string(fd_));
  CloseTcpSocket(fd_);
}

AsyncBind TcpListener::Bind(EndPoint end_point) const noexcept {
  return AsyncBind(fd_, std::move(end_point), settings_.back_log);
}

AsyncAccept TcpListener::Accept() const noexcept { return AsyncAccept(fd_, settings_.accept_timeout); }

}  // namespace ntio::sockets
