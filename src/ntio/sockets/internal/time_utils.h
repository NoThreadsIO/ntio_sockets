// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_INTERNAL_TIME_UTILS_H_
#define NTIO_SOCKETS_INTERNAL_TIME_UTILS_H_

#include <sys/time.h>

#include <chrono>

namespace ntio::sockets::internal {

using SteadyClock = std::chrono::steady_clock;
timeval ConvertToTimeval(const std::chrono::milliseconds &interval);

}  // namespace ntio::sockets::internal

#endif  // NTIO_SOCKETS_INTERNAL_TIME_UTILS_H_
