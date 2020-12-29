// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_TCP_LISTENER_H_
#define NTIO_SOCKETS_TCP_LISTENER_H_

#include "ntio/sockets/async_accept.h"
#include "ntio/sockets/async_bind.h"

namespace ntio::sockets {

/**
 * Interface for TCP Listener
 */
class ITcpListener {
 public:
  virtual ~ITcpListener() = default;

  /**
   * Binds a listening socket
   *
   * @code{.cpp}
   * auto error = co_await listener.Bind(EndPoint("127.0.0.1", 5000));
   * if (error) {
   *   // error handling
   * }
   * @endcode
   *
   * @param end_point - the address and port to bind
   * @return Awaitable object which awaits Error
   */
  virtual AsyncBind Bind(EndPoint end_point) const noexcept = 0;

  /**
   * Accepts a new connection
   *
   * @note The listening socket must be bound
   *
   * @code{.cpp}
   *
   * auto [socket, error] = co_await listener.Accept();
   * if (error) {
   *   // error handling
   * } else {
   *   co_await socket->Write({1,2,3,4,5}, 5);
   * }
   *
   * @endcode
   *
   * @return Awaitable object which awaits {std::unique_ptr<ITcpSocket>, Error}, where the pointer is the new connected
   * socket
   */
  virtual AsyncAccept Accept() const noexcept = 0;
};

/**
 * Implementation of ITcpListener
 */
class TcpListener : public ITcpListener {
 public:
  /**
   * Listener's settings
   */
  struct Settings {
    size_t back_log;
    std::chrono::milliseconds accept_timeout;
  };

  static const Settings kDefaultSettings;

  explicit TcpListener(Settings settings);
  TcpListener();

  ~TcpListener() override;
  AsyncBind Bind(EndPoint end_point) const noexcept override;
  AsyncAccept Accept() const noexcept override;

 private:
  int fd_;
  Settings settings_;
};

}  // namespace ntio::sockets
#endif  // NTIO_SOCKETS_TCP_LISTENER_H_
