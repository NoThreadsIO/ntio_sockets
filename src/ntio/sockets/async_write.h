// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_ASYNC_WRITE_H_
#define NTIO_SOCKETS_ASYNC_WRITE_H_

#include <chrono>
#include <coroutine>
#include <vector>

#include "error.h"
#include "ntio/sockets/internal/time_utils.h"

namespace ntio::sockets {
class AsyncWrite {
 public:
  struct Result {
    size_t bytes;
    Error error;
  };

  struct Awaiter {
    explicit Awaiter(const AsyncWrite* owner);
    bool await_ready() noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    Result await_resume() noexcept;

    const AsyncWrite* owner;
    Error error;
  };

  AsyncWrite(int fd, std::chrono::milliseconds timeout, const std::vector<uint8_t>& data, size_t len);
  Awaiter operator co_await() const noexcept;

 private:
  int fd_;
  std::chrono::milliseconds timeout_;
  internal::SteadyClock::time_point start_time_;
  const std::vector<uint8_t>& data_;
  size_t len_;
};
}  // namespace ntio::sockets

#endif  // NTIO_SOCKETS_ASYNC_WRITE_H_
