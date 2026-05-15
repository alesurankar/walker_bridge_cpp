#include "walker_bridge_cpp/udp_receiver.hpp"
#include <nlohmann/json.hpp>


using boost::asio::ip::udp;

UdpReceiver::UdpReceiver()
  : 
  Node("udp_receiver"),
  socket_(io_context_, udp::endpoint(udp::v4(), PORT)),
  work_guard_(boost::asio::make_work_guard(io_context_))
{
  pub_ = this->create_publisher<std_msgs::msg::Float32MultiArray>(
    "/humanoid/joint_targets", 10);

  RCLCPP_INFO(this->get_logger(), "UDP Receiver started on port %d", PORT);

  start_receive();

  io_thread_ = std::thread([this]() {
    io_context_.run();
  });
}

UdpReceiver::~UdpReceiver()
{
  io_context_.stop();

  if (io_thread_.joinable())
    io_thread_.join();
}

void UdpReceiver::start_receive()
{
  auto buffer = std::make_shared<std::array<char, 65536>>();

  socket_.async_receive_from(
    boost::asio::buffer(*buffer),
    remote_endpoint_,
    [this, buffer](const boost::system::error_code &ec, std::size_t bytes)
    {
      if (!ec && bytes > 0)
      {
        try
        {
          std::string data(buffer->data(), bytes);

          auto j = nlohmann::json::parse(data);

          std_msgs::msg::Float32MultiArray msg;
          msg.data.clear();

          for (auto &v : j["joint_positions"])
          {
            msg.data.push_back(v.get<float>());
          }

          pub_->publish(msg);
        }
        catch (const std::exception &e)
        {
          RCLCPP_ERROR(this->get_logger(), "JSON parse error: %s", e.what());
        }
      }
      start_receive();
    });
}