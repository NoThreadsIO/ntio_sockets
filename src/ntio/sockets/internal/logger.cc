// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "logger.h"

#include <iostream>
#include <map>

namespace ntio::sockets::internal {

void Log(int level, int line, const std::string& file, const std::string& message) {
  static const std::map<int, const char*> kLevelMap{
      {0, "ERROR"},
      {1, "WARN"},
      {2, "INFO"},
      {3, "DEBUG"},
  };
  std::cout << "[" << kLevelMap.at(level) << "] " << file << ":" << line << "\t" << message << std::endl;
}
}  // namespace ntio::sockets::internal
