// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_TCP_LISTENER_H
#define NTIO_SOCKETS_TCP_LISTENER_H

#include "ntio/sockets/async_accept.h"
#include "ntio/sockets/async_bind.h"

namespace ntio::sockets {


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


}
#endif  // NTIO_SOCKETS_TCP_LISTENER_H
