// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_END_POINT_H_
#define NTIO_SOCKETS_END_POINT_H_

#include <netinet/in.h>

#include <ostream>
#include <string>

namespace ntio::sockets {

class EndPoint {
 public:
  EndPoint(std::string host, int port);
  explicit EndPoint(struct sockaddr_in addr);

  const std::string &host() const;
  int port() const;
  const sockaddr_in &addr() const;

  explicit operator std::string() const;

  bool operator==(const EndPoint &rhs) const;
  bool operator!=(const EndPoint &rhs) const;
  bool operator<(const EndPoint &rhs) const;

  friend std::ostream &operator<<(std::ostream &os, const EndPoint &point);

 private:
  std::string host_;
  int port_;
  struct sockaddr_in addr_;
};
}  // namespace ntio::sockets

#endif  // NTIO_SOCKETS_END_POINT_H_
