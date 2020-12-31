// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_ASYNC_CONNECT_H_
#define NTIO_SOCKETS_ASYNC_CONNECT_H_

#include <chrono>
#include <coroutine>

#include "end_point.h"
#include "ntio/core/error.h"
#include "ntio/sockets/internal/time_utils.h"

namespace ntio::sockets {

class AsyncConnect {
 public:
  struct Awaiter {
    Awaiter(const AsyncConnect* owner, core::Error error);
    bool await_ready() noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    core::Error await_resume() noexcept;

    const AsyncConnect* owner;
    core::Error error;
  };

  AsyncConnect(int fd, EndPoint end_point, std::chrono::milliseconds timeout);
  Awaiter operator co_await() const noexcept;

 private:
  int fd_;
  std::chrono::milliseconds timeout_;
  EndPoint end_point_;
  internal::SteadyClock::time_point start_time_;
  core::Error error_;
};
}  // namespace ntio::sockets

#endif  // NTIO_SOCKETS_ASYNC_CONNECT_H_
