// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_ASYNC_READ_H_
#define NTIO_SOCKETS_ASYNC_READ_H_

#include <sys/select.h>

#include <chrono>
#include <coroutine>
#include <cstdint>
#include <vector>

#include "ntio/core/error.h"
#include "ntio/sockets/internal/time_utils.h"

namespace ntio::sockets {

class AsyncRead {
 public:
  struct Result {
    size_t bytes;
    core::Error error;
  };

  struct Awaiter {
    explicit Awaiter(const AsyncRead* owner);
    bool await_ready() noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    Result await_resume() noexcept;

    const AsyncRead* owner;
    core::Error error;
  };

  AsyncRead(int fd, std::chrono::milliseconds timeout, std::vector<uint8_t>* data, size_t len);
  Awaiter operator co_await() const noexcept;

 private:
  int fd_;
  std::chrono::milliseconds timeout_;
  internal::SteadyClock::time_point start_time_;
  std::vector<uint8_t>* data_;
  size_t len_;
};

}  // namespace ntio::sockets

#endif  // NTIO_SOCKETS_ASYNC_READ_H_
