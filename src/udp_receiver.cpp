#include "walker_bridge_cpp/udp_receiver.hpp"
#include <iostream>


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
  
  if (io_thread_.joinable()) {
    io_thread_.join();
  }

  std::cout << "[UdpReceiver] destroyed" << std::endl;
}

void UdpReceiver::start()
{
  std::cout << "[UdpReceiver] start()" << std::endl;
  running_ = true;

  if (!socket_.is_open()) {
    socket_.open(udp::v4());
  }
  socket_.bind(udp::endpoint(
    udp::v4(),
    port_
  ));
    io_thread_ = std::thread([this]() {
    io_context_.run();
  });

  start_receive();
}

void UdpReceiver::stop()
{
  if (!running_) {
    return;
  }

  std::cout << "[UdpReceiver] stop()" << std::endl;
  running_ = false;
  io_context_.stop();
}

void UdpReceiver::set_callback(MessageCallback cb)
{
  callback_ = cb;
  std::cout << "[UdpReceiver] callback set" << std::endl;
}

void UdpReceiver::io_loop()
{
  std::cout << "[UdpReceiver] io_loop()" << std::endl;
}

void UdpReceiver::start_receive()
{
  std::cout << "=============================" << std::endl;
  std::cout << "[UdpReceiver] start_receive()" << std::endl;
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
  if (error) {
    return;
  }

  std::string msg(buffer_.data(), bytes_received);
  std::cout << "[UdpReceiver] msg: " << msg << std::endl;

  if (callback_) {
    callback_(msg);
  }

  std::cout << "[UdpReceiver] handle_receive() bytes: " << bytes_received << std::endl;
}
