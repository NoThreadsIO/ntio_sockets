// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_ERROR_H_
#define NTIO_SOCKETS_ERROR_H_

#include <ostream>
namespace ntio::sockets {

struct Error {
  Error() = default;
  explicit Error(int err);
  int code = 0;
  std::string message;

  operator int() const noexcept;
  operator bool() const noexcept;
  operator std::string() const noexcept;

  friend std::ostream& operator<<(std::ostream& os, const Error& error);
};

}  // namespace ntio::sockets
#endif  // NTIO_SOCKETS_ERROR_H_
