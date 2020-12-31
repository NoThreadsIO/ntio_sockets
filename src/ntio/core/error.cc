// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "error.h"

namespace ntio::core {

std::ostream& operator<<(std::ostream& os, const Error& error) {
  os << static_cast<std::string>(error);
  return os;
}

Error::Error(int err) : code(err), message(std::generic_category().message(err)) {}

Error::operator int() const noexcept { return code; }

Error::operator std::string() const noexcept { return message + "[" + std::to_string(code) + "]"; }

Error::operator bool() const noexcept { return code != 0; }

}  // namespace ntio::core
