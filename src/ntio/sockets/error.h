// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_ERROR_H_
#define NTIO_SOCKETS_ERROR_H_

#include <ostream>
namespace ntio::sockets {

/**
 * Socket error
 */
struct Error {
  /**
   * No error
   */
  Error() = default;

  /**
   * Creates an error
   * @param err errno code
   */
  explicit Error(int err);
  int code = 0;
  std::string message;

  /**
   * @return errno code
   */
  operator int() const noexcept;

  /**
   * @return true if there is an error
   */
  operator bool() const noexcept;

  /**
   * @return error message
   */
  operator std::string() const noexcept;

  /**
   * Prints error message with its code
   * @param os
   * @param error
   * @return
   */
  friend std::ostream& operator<<(std::ostream& os, const Error& error);
};

}  // namespace ntio::sockets
#endif  // NTIO_SOCKETS_ERROR_H_
