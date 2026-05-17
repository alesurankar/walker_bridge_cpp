#pragma once
#include "udp_ros_bridge/protocol/udp_message.hpp"
#include "udp_ros_bridge/protocol/command_message.hpp"
#include <optional>
#include <cstddef>


class CommandDecoder
{
public:
  static std::optional<udp_ros_bridge::CommandMessage>
  decode(const std::byte* data, std::size_t size);
};