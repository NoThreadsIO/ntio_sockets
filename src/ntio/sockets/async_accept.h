// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_ASYNC_ACCEPT_H_
#define NTIO_SOCKETS_ASYNC_ACCEPT_H_

#include <chrono>
#include <coroutine>
#include <memory>

#include "ntio/core/error.h"
#include "ntio/sockets/internal/time_utils.h"

namespace ntio::sockets {

class ITcpSocket;

class AsyncAccept {
 public:
  struct Result {
    std::unique_ptr<ITcpSocket> connected_socket;
    core::Error error;
  };

  struct Awaiter {
    explicit Awaiter(const AsyncAccept* owner);
    bool await_ready() noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    Result await_resume() noexcept;

    const AsyncAccept* owner;
    core::Error error;
  };

  AsyncAccept(int fd, std::chrono::milliseconds timeout);
  Awaiter operator co_await() const noexcept;

 private:
  int fd_;
  std::chrono::milliseconds timeout_;
  internal::SteadyClock::time_point start_time_;
};
}  // namespace ntio::sockets
#endif  // NTIO_SOCKETS_ASYNC_ACCEPT_H_
