// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include <ntio/core/error.h>
#include <ntio/core/logger.h>
#include <ntio/core/task.h>
#include <ntio/sockets/tcp_listener.h>
#include <ntio/sockets/tcp_socket.h>

#include <coroutine>
#include <iostream>
#include <memory>

static const int kPort = 6545;

using ntio::core::Error;
using Task = ntio::core::Task<Error>;

Task PingPong() {
  using ntio::sockets::EndPoint;
  using ntio::sockets::TcpListener;
  using ntio::sockets::TcpSocket;

  TcpListener listener;
  if (auto err = co_await listener.Bind(EndPoint("127.0.0.1", kPort))) {
    LOG_ERROR("Bind error: " << err);
    co_return err;
  }

  auto accept_task = listener.Accept();

  TcpSocket client;
  auto con_task = client.Connect(EndPoint("127.0.0.1", kPort));

  if (auto err = co_await con_task) {
    LOG_ERROR("Conn error: " << err);
    co_return err;
  }

  auto [sock, accept_err] = co_await accept_task;
  if (accept_err) {
    LOG_ERROR("Accept error: " << accept_err);
    co_return accept_err;
  }

  std::vector<uint8_t> rbuffer(1'000'000);
  std::vector<uint8_t> wbuffer(1'000'000);

  auto [write_bytes, write_err] = co_await sock->Write(wbuffer, 1'000'000);
  if (write_err) {
    LOG_ERROR("Write error: " << write_err);
    co_return write_err;
  }

  LOG_INFO("Write " << write_bytes << " bytes");

  auto [read_bytes, read_err] = co_await client.Read(&rbuffer, 1'000'000);
  if (read_err) {
    LOG_ERROR("Read error: " << read_err);
    co_return read_err;
  }

  LOG_INFO("Read " << read_bytes << " bytes");
  co_return Error();
}

int main() {
  auto ret = PingPong();
  LOG_INFO("Result: " << ret);
  return ret ? -1 : 0;
}
