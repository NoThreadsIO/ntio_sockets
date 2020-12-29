// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#ifndef NTIO_SOCKETS_INTERNAL_SOCKET_UTILS_H_
#define NTIO_SOCKETS_INTERNAL_SOCKET_UTILS_H_

#include <chrono>
#include <tuple>
#include <vector>

#include "ntio/sockets/error.h"

namespace ntio::sockets::internal {

Error CheckConnection(const int fd, std::chrono::milliseconds timeout);

Error CheckDataToRead(int fd, std::chrono::milliseconds timeout);

Error CheckDataToWrite(int fd, std::chrono::milliseconds timeout);

std::tuple<size_t, Error> ReadData(int fd, std::vector<uint8_t>* data, size_t len);

std::tuple<size_t, Error> WriteData(int fd, const std::vector<uint8_t>& data, size_t len);

int OpenTcpSocket();

void CloseTcpSocket(int fd);

}  // namespace ntio::sockets::internal

#endif  // NTIO_SOCKETS_INTERNAL_SOCKET_UTILS_H_
