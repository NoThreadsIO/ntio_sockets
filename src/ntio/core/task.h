// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_CORE_TASK_H_
#define NTIO_CORE_TASK_H_

#include <ntio/core/logger.h>

#include <coroutine>

namespace ntio::core {
/**
 * Task
 * @tparam T
 */
template <typename T>
struct Task {
  struct promise_type {
    std::suspend_never initial_suspend() {
      return {};
    }

    void unhandled_exception() { }

    std::suspend_never final_suspend() noexcept {
      return {};
    }

    Task get_return_object() {
      Task<T> task;
      state = &task.value;
      return task;
    }

    void return_value(const T& value) { *state = value; }

    T* state;
  };

  friend std::ostream& operator<<(std::ostream& os, const Task& task) {
    os << task.value;
    return os;
  }

  template <class U>
  operator U() { return static_cast<U>(value); }

  T value;
};

}  // namespace ntio::core
#endif  // NTIO_CORE_TASK_H_
