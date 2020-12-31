// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include "socket_utils.h"

#include <fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <system_error>

#include "ntio/core/logger.h"
#include "time_utils.h"

namespace ntio::sockets::internal {

using ::ntio::core::Error;

Error CheckConnection(const int fd, std::chrono::milliseconds timeout) {
  auto start_time = SteadyClock::now();
  Error error;
  while (!error) {
    auto tv = ConvertToTimeval(timeout);

    fd_set myset;
    FD_ZERO(&myset);
    FD_SET(fd, &myset);

    auto res = select(fd + 1, NULL, &myset, NULL, &tv);

    if (res < 0 && errno != EINTR) {
      error = Error(errno);
      LOG_ERROR("select: " << error);

    } else if (res > 0) {
      // Socket selected for write
      int err;
      socklen_t lon = sizeof(int);
      if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &lon) < 0) {
        error = Error(errno);
        LOG_ERROR("getsockopt: " << error);
      }
      // Check the value returned...
      if (err) {
        error = Error(err);
        LOG_ERROR("getsockopt: " << error);
      }
      break;
    } else {
      error = Error(ETIMEDOUT);
      LOG_ERROR(error);
    }
  }

  return error;
}

Error CheckDataToRead(int fd, std::chrono::milliseconds timeout) {
  fd_set rfds;
  FD_ZERO(&rfds);
  FD_SET(fd, &rfds);
  auto tv = ConvertToTimeval(timeout);

  auto retval = select(fd + 1, &rfds, NULL, NULL, &tv);
  if (retval == -1) {
    Error err(errno);
    LOG_ERROR("select: " << err);
    return err;
  } else if (retval > 0) {
    return Error();
  }
  return Error(ETIMEDOUT);
}

Error CheckDataToWrite(int fd, std::chrono::milliseconds timeout) {
  fd_set wfds;
  FD_ZERO(&wfds);
  FD_SET(fd, &wfds);
  auto tv = ConvertToTimeval(timeout);

  auto retval = select(fd + 1, NULL, &wfds, NULL, &tv);
  if (retval == -1) {
    Error err(errno);
    LOG_ERROR("select: " << err);
    return err;
  } else if (retval > 0) {
    return Error();
  }
  return Error(ETIMEDOUT);
}

std::tuple<size_t, Error> ReadData(int fd, std::vector<uint8_t>* data, size_t len) {
  Error error;
  int n = recv(fd, data->data(), len, 0);
  if (n < 0) {
    error = Error(errno);
    LOG_ERROR("recv: " << error);
    n = 0;
  }

  return std::make_tuple(n, std::move(error));
}

std::tuple<size_t, Error> WriteData(int fd, const std::vector<uint8_t>& data, size_t len) {
  Error error;
  int n = send(fd, data.data(), len, 0);
  if (n < 0) {
    error = Error(errno);
    LOG_ERROR("send: " << error);
    n = 0;
  }

  return std::make_tuple(n, std::move(error));
}

int OpenTcpSocket() {
  auto fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  // Set non-blocking
  auto arg = fcntl(fd, F_GETFL, NULL);
  if (arg < 0) {
    throw std::system_error(errno, std::generic_category());
  }

  arg |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, arg) < 0) {
    throw std::system_error(errno, std::generic_category());
  }
  return fd;
}

void CloseTcpSocket(int fd) {
  shutdown(fd, SHUT_RDWR);
  close(fd);
}

}  // namespace ntio::sockets::internal
