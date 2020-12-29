// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "async_write.h"

#include "ntio/sockets/internal/socket_utils.h"
#include "ntio/sockets/internal/time_utils.h"

namespace ntio::sockets {

using internal::CheckDataToWrite;
using internal::SteadyClock;
using internal::WriteData;

/*
 * AsyncWrite::Awaiter
 */
AsyncWrite::Awaiter::Awaiter(const AsyncWrite* owner) : owner(owner) {}

bool AsyncWrite::Awaiter::await_ready() noexcept {
  auto err = CheckDataToWrite(owner->fd_, std::chrono::milliseconds(0));
  if (!err) {
    return true;
  } else if (err.code == ETIMEDOUT) {
    return false;  // wait for data
  } else {
    error = std::move(err);
    return true;
  }
}

void AsyncWrite::Awaiter::await_suspend(std::coroutine_handle<> handle) noexcept {
  auto& [fd, timeout, start_time, _data, _len] = *owner;
  error = CheckDataToWrite(
      fd, timeout - std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - start_time));

  handle.resume();
}

AsyncWrite::Result AsyncWrite::Awaiter::await_resume() noexcept {
  Result ret{
      .bytes = 0,
      .error = std::move(error),
  };

  if (!ret.error) {
    auto& [fd, _timeout, _start_time, data, len] = *owner;
    const auto [n, err] = WriteData(fd, data, len);
    ret = {
        .bytes = n,
        .error = err,
    };
  }

  return ret;
}

/*
 * AsyncWrite
 */

AsyncWrite::AsyncWrite(int fd, std::chrono::milliseconds timeout, const std::vector<uint8_t>& data, size_t len)
    : fd_(fd), timeout_(timeout), start_time_(SteadyClock::now()), data_(data), len_(len) {}

AsyncWrite::Awaiter AsyncWrite::operator co_await() const noexcept { return Awaiter(this); }

}  // namespace ntio::sockets
