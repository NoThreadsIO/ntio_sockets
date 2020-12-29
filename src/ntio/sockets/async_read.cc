// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "async_read.h"

#include <chrono>

#include "ntio/sockets/internal/logger.h"
#include "ntio/sockets/internal/socket_utils.h"
#include "ntio/sockets/internal/time_utils.h"

namespace ntio::sockets {

using ntio::sockets::internal::CheckDataToRead;
using ntio::sockets::internal::ReadData;
using ntio::sockets::internal::SteadyClock;

/*
 * AsyncRead::Awaiter
 */
bool AsyncRead::Awaiter::await_ready() noexcept {
  auto err = CheckDataToRead(owner->fd_, std::chrono::milliseconds(0));
  if (!err) {
    return true;
  } else if (err.code == ETIMEDOUT) {
    return false;  // wait for data
  } else {
    error = std::move(err);
    return true;
  }
}

void AsyncRead::Awaiter::await_suspend(std::coroutine_handle<> handle) noexcept {
  auto& [fd, timeout, start_time, _data, _len] = *owner;
  error = CheckDataToRead(
      fd, timeout - std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - start_time));

  handle.resume();
}

AsyncRead::Result AsyncRead::Awaiter::await_resume() noexcept {
  Result ret{
      .bytes = 0,
      .error = std::move(error),
  };

  if (!ret.error) {
    auto& [fd, _timeout, _start_time, data, len] = *owner;
    const auto [n, err] = ReadData(fd, data, len);
    ret = {
        .bytes = n,
        .error = err,
    };
  }

  return ret;
}

AsyncRead::Awaiter::Awaiter(const AsyncRead* owner) : owner(owner), error() {}

/*
 * AsyncRead
 */
AsyncRead::AsyncRead(int fd, std::chrono::milliseconds timeout, std::vector<uint8_t>* data, size_t len)
    : fd_(fd), timeout_(timeout), start_time_(SteadyClock::now()), data_(data), len_(len) {}

AsyncRead::Awaiter AsyncRead::operator co_await() const noexcept { return Awaiter(this); }

}  // namespace ntio::sockets
