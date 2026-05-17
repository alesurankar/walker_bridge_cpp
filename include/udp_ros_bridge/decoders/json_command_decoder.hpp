#pragma once
#include "udp_ros_bridge/decoders/command_decoder.hpp"


namespace udp_ros_bridge
{

class JsonCommandDecoder
{
public:
  static std::optional<CommandMessage>
  decode(const std::byte* data, std::size_t size);
};

}