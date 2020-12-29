// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_TCP_SOCKET_H_
#define NTIO_SOCKETS_TCP_SOCKET_H_

#include <vector>

#include "ntio/sockets/async_connect.h"
#include "ntio/sockets/async_read.h"
#include "ntio/sockets/async_write.h"
#include "ntio/sockets/end_point.h"

namespace ntio::sockets {

/**
 * Interface for TCP socket
 */
class ITcpSocket {
 public:
  virtual ~ITcpSocket() = default;

  /**
   * Connects to a server
   *
   * @code{.cpp}
   *    auto error = co_await socket.Connect(EndPoint("127.0.0.1", 4000));
   * @endcode
   *
   * @param end_point the server's address
   * @return Awaitable object which awaits Error
   */
  virtual AsyncConnect Connect(EndPoint end_point) const noexcept = 0;

  /**
   * Reads data
   *
   * @note The socket must be connected
   *
   * @code{.cpp}
   *    std::vector<uint8_t> buffer(10);
   *    auto [n, error] = co_await socket.Read(&buffer, 10);
   *    if (error) {
   *      // error handling
   *    }
   * @endcode
   *
   *
   * @param data - pointer to input buffer.
   * @param len - length of the expected. Must be less or equal data.size().
   * @return Awaitable object which awaits {size_t, Error},
   * where size_t is the number of read bytes
   */
  virtual AsyncRead Read(std::vector<uint8_t>* data, size_t len) const noexcept = 0;

  /**
   * Writes data
   * @note The socket must be connected
   *
   * @code{.cpp}
   *    std::vector<uint8_t> data = {1,2,3,4,5};
   *    auto [n, error] = co_await socket.Write(data, 5);
   *    if (error) {
   *      // error handling
   *    }
   * @endcode
   * @param data - data to send.
   * @param len - the length of the data. Must be less or equal data.size().
   * @return Awaitable object which awaits {size_t, Error},
   * where size_t is the number of written bytes
   */
  virtual AsyncWrite Write(const std::vector<uint8_t>& data, size_t len) const noexcept = 0;
};

/**
 * Implementation of ITcpSocket
 */
class TcpSocket : public ITcpSocket {
 public:
  /**
   * Socket's settings
   */
  struct Settings {
    std::chrono::milliseconds connection_timeout;
    std::chrono::milliseconds read_timeout;
    std::chrono::milliseconds write_timeout;
  };

  /**
   * Default settings - all the timeouts are 1 second
   */
  static const Settings kDefaultSettings;

  TcpSocket();
  explicit TcpSocket(Settings settings);
  TcpSocket(int fd, Settings settings);

  ~TcpSocket() override;

  AsyncConnect Connect(EndPoint end_point) const noexcept override;
  AsyncRead Read(std::vector<uint8_t>* data, size_t len) const noexcept override;
  AsyncWrite Write(const std::vector<uint8_t>& data, size_t len) const noexcept override;

  [[nodiscard]] const Settings& settings() const { return settings_; }
  void set_settings(const Settings& settings) { settings_ = settings; }

 private:
  int fd_;
  Settings settings_;
};

}  // namespace ntio::sockets
#endif  // NTIO_SOCKETS_TCP_SOCKET_H_
