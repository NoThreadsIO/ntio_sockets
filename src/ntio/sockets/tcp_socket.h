// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_TCP_SOCKET_H_
#define NTIO_SOCKETS_TCP_SOCKET_H_

#include <vector>

#include "async_accept.h"
#include "async_bind.h"
#include "async_connect.h"
#include "async_read.h"
#include "async_write.h"
#include "end_point.h"

namespace ntio::sockets {

class ITcpSocket {
 public:
  virtual ~ITcpSocket() = default;
  virtual AsyncConnect Connect(EndPoint end_point) const noexcept = 0;
  virtual AsyncRead Read(std::vector<uint8_t>* data, size_t len) const noexcept = 0;
  virtual AsyncWrite Write(const std::vector<uint8_t>& data, size_t len) const noexcept = 0;
};

class TcpSocket : public ITcpSocket {
 public:
  struct Settings {
    std::chrono::milliseconds connection_timeout;
    std::chrono::milliseconds read_timeout;
    std::chrono::milliseconds write_timeout;
  };

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

class ITcpListener {
 public:
  virtual ~ITcpListener() = default;

  virtual AsyncBind Bind(EndPoint end_point) const noexcept = 0;
  virtual AsyncAccept Accept() const noexcept = 0;
};

class TcpListener : public ITcpListener {
 public:
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
#endif  // NTIO_SOCKETS_TCP_SOCKET_H_
