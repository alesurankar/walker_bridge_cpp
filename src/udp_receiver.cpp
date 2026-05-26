#include "udp_ros_bridge/udp_receiver.hpp"
#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>


using udp = boost::asio::ip::udp;

UdpReceiver::UdpReceiver(uint16_t port)
  : 
  port_(port),
  io_context_(),
  work_guard_(boost::asio::make_work_guard(io_context_)),
  socket_(io_context_)
{
  std::cout << "[UdpReceiver] constructed on port " << port_ << std::endl;
}

UdpReceiver::~UdpReceiver()
{
  stop();
  std::cout << "[UdpReceiver] destroyed" << std::endl;
}

void UdpReceiver::start()
{
  std::cout << "[UdpReceiver] start()" << std::endl;
  running_ = true;

  if (!socket_.is_open()) {
    socket_.open(udp::v4());
  }
  
  boost::asio::socket_base::reuse_address option(true);
  socket_.set_option(option);

  socket_.bind(udp::endpoint(
    udp::v4(),
    //boost::asio::ip::make_address("127.0.0.1"),
    port_
  ));

  start_receive();
  io_thread_ = std::thread([this]() {
    io_context_.run();
  });

}

void UdpReceiver::stop()
{
  if (!running_) {
    return;
  }

  std::cout << "[UdpReceiver] stop()" << std::endl;
  running_ = false;

  boost::system::error_code ec;
  socket_.cancel(ec);
  socket_.close(ec);
  work_guard_.reset();
  io_context_.stop();

  if (io_thread_.joinable()) {
    io_thread_.join();
  }
}

bool UdpReceiver::pop_message(UdpMessage& out_msg)
{
  return queue_.pop(out_msg);
}

void UdpReceiver::start_receive()
{
  socket_.async_receive_from(
    boost::asio::buffer(buffer_),
    remote_endpoint_,
    [this](const boost::system::error_code& error, std::size_t bytes_received)
    {
      handle_receive(error, bytes_received);

      if (running_) {
        start_receive();
      }
    }
  );
}

void UdpReceiver::handle_receive(const boost::system::error_code& error, std::size_t bytes_received)
{
  if (error == boost::asio::error::operation_aborted) {
    return;
  }

  if (error) {
    std::cerr << "[UdpReceiver] error: " << error.message() << std::endl;
    return;
  }

  UdpMessage msg;
  const auto now = std::chrono::steady_clock::now();
  msg.receive_timestamp_ns =
    std::chrono::duration_cast<std::chrono::nanoseconds>(
      now.time_since_epoch()).count();

  const std::size_t size = (bytes_received > MAX_MSG_SIZE)
    ? MAX_MSG_SIZE
    : bytes_received;

  msg.size = static_cast<uint16_t>(size);

  std::memcpy(msg.data, buffer_.data(), size);

  if (!queue_.push(msg)) {
    drop_count_.fetch_add(1, std::memory_order_relaxed);
    //std::cout << "[QUEUE] DROP" << std::endl;
  }
  else {
    //std::cout << "==============================\n";
    //std::cout << "[QUEUE] push OK, size=" << msg.size << std::endl;
  }
}