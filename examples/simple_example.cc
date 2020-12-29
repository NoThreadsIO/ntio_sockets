// Copyright (c) 2020 Aleksey Timin <atimin@gmail.com>
// Licensed under the MIT License.

#include <ntio/sockets/tcp_socket.h>

#include <coroutine>
#include <iostream>
#include <memory>

static const int kPort = 6545;
struct Promise;

struct Task {
  using promise_type = Promise;

  explicit Task(int& state) : state_(state) {}
  int& state_;

  operator int() { return state_; }
};

struct Promise {
  Task get_return_object() {
    std::cout << "Promise.get_return_object()" << std::endl;
    return Task(state_);
  }

  std::suspend_never initial_suspend() {
    std::cout << "Promise.initial_suspend()" << std::endl;
    return {};
  }

  std::suspend_never final_suspend() noexcept {
    std::cout << "Promise.final_suspend()" << std::endl;
    return {};
  }

  void return_void() { std::cout << "Promise.return_void()" << std::endl; }

  void return_value(const int& val) {
    std::cout << "Promise.return_value() with " << val << std::endl;
    state_ = val;
  }

  std::suspend_never yield_value(const int& val) {
    std::cout << "Promise.yield_value() with " << val << std::endl;
    return {};
  }

  void unhandled_exception() { std::cout << "Promise.unhandled_exception()" << std::endl; }

  int state_;
};

Task PingPong() {
  using ntio::sockets::EndPoint;
  using ntio::sockets::TcpListener;
  using ntio::sockets::TcpSocket;

  TcpListener listener;
  if (auto err = co_await listener.Bind(EndPoint("127.0.0.1", kPort))) {
    std::cout << "Bind error: " << err << std::endl;
    co_return;
  }

  auto accept_task = listener.Accept();

  TcpSocket client;
  auto con_task = client.Connect(EndPoint("127.0.0.1", kPort));

  if (auto err = co_await con_task) {
    std::cout << "Conn error: " << err << std::endl;
    co_return;
  }

  auto [sock, accept_err] = co_await accept_task;
  if (accept_err) {
    std::cout << "Accept error: " << accept_err << std::endl;
    co_return;
  }

  std::vector<uint8_t> rbuffer(1'000'000);
  std::vector<uint8_t> wbuffer(1'000'000);

  auto [write_bytes, write_err] = co_await sock->Write(wbuffer, 1'000'000);
  if (write_err) {
    std::cout << "Write error: " << write_err << std::endl;
    co_return;
  }

  std::cout << "Write " << write_bytes << " bytes" << std::endl;

  auto [read_bytes, read_err] = co_await client.Read(&rbuffer, 1'000'000);
  if (read_err) {
    std::cout << "Read error: " << read_err << std::endl;
    co_return;
  }

  std::cout << "Read " << read_bytes << " bytes" << std::endl;
}

int main() {
  PingPong();
  return 0;
}
