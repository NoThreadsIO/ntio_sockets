// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "end_point.h"

#include <arpa/inet.h>

#include <system_error>
#include <utility>

namespace ntio::sockets {
EndPoint::EndPoint(struct sockaddr_in addr)
    : host_(inet_ntoa(addr.sin_addr)), port_(htons(addr.sin_port)), addr_(addr) {}

EndPoint::EndPoint(std::string host, int port) : host_(std::move(host)), port_(port), addr_{0} {
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port_);
  if (inet_aton(host_.c_str(), &addr_.sin_addr) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
}

const std::string &EndPoint::host() const { return host_; }

int EndPoint::port() const { return port_; }

const sockaddr_in &EndPoint::addr() const { return addr_; }

bool EndPoint::operator==(const EndPoint &rhs) const {
  return host_ == rhs.host_ && port_ == rhs.port_ && addr_.sin_addr.s_addr == rhs.addr_.sin_addr.s_addr &&
         addr_.sin_port == rhs.addr_.sin_port && addr_.sin_family == rhs.addr_.sin_family;
}

bool EndPoint::operator!=(const EndPoint &rhs) const { return !(rhs == *this); }

bool EndPoint::operator<(const EndPoint &rhs) const { return host_ < rhs.host_ && port_ < rhs.port_; }

EndPoint::operator std::string() const { return host_ + ":" + std::to_string(port_); }
std::ostream &operator<<(std::ostream &os, const EndPoint &point) {
  os << point.host_ << ":" << point.port_;
  return os;
}
}  // namespace ntio::sockets
