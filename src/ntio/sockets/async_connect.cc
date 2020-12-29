// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "async_connect.h"

#include <chrono>

#include "ntio/sockets/internal/logger.h"
#include "ntio/sockets/internal/socket_utils.h"
#include "ntio/sockets/internal/time_utils.h"

namespace ntio::sockets {

using internal::CheckConnection;
using internal::SteadyClock;

/*
 * AsyncRead::Awaiter
 */
AsyncConnect::Awaiter::Awaiter(const AsyncConnect *owner, Error error) : owner(owner), error(error) {}

bool AsyncConnect::Awaiter::await_ready() noexcept { return error; }

void AsyncConnect::Awaiter::await_suspend(std::coroutine_handle<> handle) noexcept {
  auto timeout =
      owner->timeout_ - std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - owner->start_time_);
  error = CheckConnection(owner->fd_, owner->timeout_);
  handle.resume();
}

Error AsyncConnect::Awaiter::await_resume() noexcept { return error; }

/*
 * AsyncRead
 */
AsyncConnect::AsyncConnect(int fd, EndPoint end_point, std::chrono::milliseconds timeout)
    : fd_(fd), end_point_(std::move(end_point)), timeout_(timeout), start_time_(SteadyClock::now()), error_{} {
  LOG_INFO("Connecting to " << end_point_ << " ...");

  auto addr = end_point_.addr();
  auto res = connect(fd_, (struct sockaddr *)&addr, sizeof(addr));
  if (res < 0) {
    if (errno != EINPROGRESS) {
      error_ = Error(errno);
      LOG_ERROR(error_)
    }
  }
}

AsyncConnect::Awaiter AsyncConnect::operator co_await() const noexcept { return Awaiter(this, error_); }
}  // namespace ntio::sockets
