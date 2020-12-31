// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "async_bind.h"

#include "ntio/core/logger.h"

namespace ntio::sockets {

using core::Error;
/*
 * AsyncBind::Awaiter
 */
AsyncBind::Awaiter::Awaiter(Error error) : error(std::move(error)) {}

bool AsyncBind::Awaiter::await_ready() noexcept { return true; }

void AsyncBind::Awaiter::await_suspend(std::coroutine_handle<> handle) noexcept {}

Error AsyncBind::Awaiter::await_resume() noexcept { return error; }

/*
 * AsyncBind
 */
AsyncBind::AsyncBind(int fd, EndPoint end_point, int back_log)
    : fd_(fd), end_point_(std::move(end_point)), back_log_(back_log) {}

AsyncBind::Awaiter AsyncBind::operator co_await() const noexcept {
  auto addr = end_point_.addr();

  LOG_INFO("Bind listen_fd=" << fd_ << " on addr " << end_point_);

  Error error;
  if (bind(fd_, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    error = Error(errno);
    LOG_ERROR("bind: " << error);
  }

  if (listen(fd_, back_log_) == -1) {
    error = Error(errno);
    LOG_ERROR("listen: " << error);
  }

  return Awaiter(std::move(error));
}
}  // namespace ntio::sockets
