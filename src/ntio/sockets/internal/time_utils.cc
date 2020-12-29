// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "time_utils.h"

#include <sys/time.h>

#include <algorithm>

namespace ntio::sockets::internal {

timeval ConvertToTimeval(const std::chrono::milliseconds &interval) {
  struct timeval tv = {
#ifdef __APPLE__
      .tv_sec = static_cast<__darwin_suseconds_t>(interval.count() / 1000),
      .tv_usec = static_cast<__darwin_suseconds_t>((interval.count() % 1000) * 1000)
#elif __linux__
      .tv_sec = static_cast<time_t>(interval.count() / 1000),
      .tv_usec = static_cast<suseconds_t>((interval.count() % 1000) * 1000)
#elif _WIN32
      // not sure what the macro is for windows
      .tv_sec = static_cast<_time64>(interval.count() / 1000),
      .tv_usec = static_cast<_time64>((interval.count() % 1000) * 1000)
#endif
  };

  tv.tv_sec = std::max<decltype(tv.tv_sec)>(tv.tv_sec, 0);
  tv.tv_usec = std::max<decltype(tv.tv_usec)>(tv.tv_usec, 0);
  return tv;
}
}  // namespace ntio::sockets::internal
