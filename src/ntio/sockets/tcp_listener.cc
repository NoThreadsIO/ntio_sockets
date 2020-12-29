// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "ntio/sockets/tcp_listener.h"

#include "ntio/sockets/internal/logger.h"
#include "ntio/sockets/internal/socket_utils.h"

namespace ntio::sockets {

using internal::CloseTcpSocket;
using internal::OpenTcpSocket;

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
