#pragma once
#include "udp_ros_bridge/protocol/command_message.hpp"

#include <optional>
#include <cstddef>


namespace udp_ros_bridge
{

class JsonCommandDecoder
{
public:
  static std::optional<CommandMessage>
  decode(const std::byte* data, std::size_t size);
};

}