// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "async_accept.h"

#include <sys/socket.h>

#include <chrono>

#include "ntio/sockets/internal/logger.h"
#include "ntio/sockets/internal/socket_utils.h"
#include "tcp_socket.h"

namespace ntio::sockets {

using internal::CheckDataToRead;
using internal::SteadyClock;

AsyncAccept::Awaiter::Awaiter(const AsyncAccept* owner) : owner(owner), error() {}

bool AsyncAccept::Awaiter::await_ready() noexcept {
  auto err = CheckDataToRead(owner->fd_, std::chrono::milliseconds(0));
  if (!err) {
    return true;
  } else if (err.code == ETIMEDOUT) {
    return false;  // wait for data
  } else {
    error = std::move(err);
    LOG_ERROR(err);
    return true;
  }
}

void AsyncAccept::Awaiter::await_suspend(std::coroutine_handle<> handle) noexcept {
  auto& [fd, timeout, start_time] = *owner;

  error = CheckDataToRead(
      owner->fd_, timeout - std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - start_time));
  handle.resume();
}

AsyncAccept::Result AsyncAccept::Awaiter::await_resume() noexcept {
  std::unique_ptr<TcpSocket> new_socket;

  if (!error) {
    struct sockaddr addr;
    socklen_t len = sizeof(addr);

    int accepted_fd = accept4(owner->fd_, &addr, &len, SOCK_NONBLOCK);
    if (accepted_fd > 0) {
      new_socket = std::make_unique<TcpSocket>(accepted_fd, TcpSocket::Settings{});
      LOG_INFO("Accepted new socket fd=" << accepted_fd);
    } else {
      error = Error(errno);
      LOG_ERROR("accept4: " << error);
    }
  }

  return {
      .connected_socket = std::move(new_socket),
      .error = std::move(error),
  };
}

AsyncAccept::AsyncAccept(int fd, std::chrono::milliseconds timeout)
    : fd_(fd), timeout_(timeout), start_time_(SteadyClock::now()) {}

AsyncAccept::Awaiter AsyncAccept::operator co_await() const noexcept {
  LOG_DEBUG("Accepting a new connection");
  return Awaiter(this);
}
}  // namespace ntio::sockets
