// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_CORE_LOGGER_H_
#define NTIO_CORE_LOGGER_H_

#include <sstream>
#include <string>

namespace ntio::core {

void Log(int level, int line, const std::string &file, const std::string &message);

constexpr const char *str_end(const char *str) { return *str ? str_end(str + 1) : str; }

constexpr bool str_slant(const char *str) { return *str == '/' ? true : (*str ? str_slant(str + 1) : false); }

constexpr const char *r_slant(const char *str) { return *str == '/' ? (str + 1) : r_slant(str - 1); }

constexpr const char *file_name(const char *str) { return str_slant(str) ? r_slant(str_end(str)) : str; }

#define LOG(level, line, file, msg)               \
  {                                               \
    std::ostringstream ss;                        \
    ss << msg;                                    \
    ntio::core::Log(level, line, file, ss.str()); \
  }

#define EMPTY_LOG(msg)     \
  {                        \
    std::ostringstream ss; \
    ss << msg;             \
  }

#define LOG_ERROR(msg) EMPTY_LOG(msg)
#define LOG_WARN(msg) EMPTY_LOG(msg)
#define LOG_INFO(msg) EMPTY_LOG(msg)
#define LOG_DEBUG(msg) EMPTY_LOG(msg)

#ifdef LOG_LEVEL_ERROR
#undef LOG_ERROR
#define LOG_ERROR(msg) LOG(0, __LINE__, core::file_name(__FILE__), msg)
#endif

#ifdef LOG_LEVEL_WARN
#undef LOG_ERROR
#define LOG_ERROR(msg) LOG(0, __LINE__, core::file_name(__FILE__), msg)
#undef LOG_WARN
#define LOG_WARN(msg) LOG(1, __LINE__, core::file_name(__FILE__), msg)
#endif

#ifdef LOG_LEVEL_INFO
#undef LOG_ERROR
#define LOG_ERROR(msg) LOG(0, __LINE__, core::file_name(__FILE__), msg)
#undef LOG_WARN
#define LOG_WARN(msg) LOG(1, __LINE__, core::file_name(__FILE__), msg)
#undef LOG_INFO
#define LOG_INFO(msg) LOG(2, __LINE__, core::file_name(__FILE__), msg)
#endif

#ifdef LOG_LEVEL_DEBUG
#undef LOG_ERROR
#define LOG_ERROR(msg) LOG(0, __LINE__, ntio::core::file_name(__FILE__), msg)
#undef LOG_WARN
#define LOG_WARN(msg) LOG(1, __LINE__, ntio::core::file_name(__FILE__), msg)
#undef LOG_INFO
#define LOG_INFO(msg) LOG(2, __LINE__, ntio::core::file_name(__FILE__), msg)
#undef LOG_DEBUG
#define LOG_DEBUG(msg) LOG(3, __LINE__, ntio::core::file_name(__FILE__), msg)
#endif

}  // namespace ntio::core

#endif  // NTIO_CORE_LOGGER_H_
