// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_ASYNC_BIND_H_
#define NTIO_SOCKETS_ASYNC_BIND_H_

#include <coroutine>

#include "end_point.h"
#include "ntio/core/error.h"

namespace ntio::sockets {

class AsyncBind {
 public:
  struct Awaiter {
    explicit Awaiter(core::Error error);
    bool await_ready() noexcept;
    void await_suspend(std::coroutine_handle<> handle) noexcept;
    core::Error await_resume() noexcept;

    core::Error error;
  };

  AsyncBind(int fd, EndPoint end_point, int back_log);
  Awaiter operator co_await() const noexcept;

 private:
  int fd_;
  EndPoint end_point_;
  int back_log_;
};

}  // namespace ntio::sockets

#endif  // NTIO_SOCKETS_ASYNC_BIND_H_
