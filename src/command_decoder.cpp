#include "udp_ros_bridge/decoders/command_decoder.hpp"
#include <iostream>
#include <cstring>


using udp_ros_bridge::CommandMessage;

std::optional<CommandMessage>
CommandDecoder::decode(const std::byte* data, std::size_t size)
{
  std::cout << "\n[DECODER] called" << std::endl;
  std::cout << "[DECODER] input size = " << size << std::endl;

  if (!data || size == 0) {
    std::cout << "[DECODER] invalid input (null or empty)" << std::endl;
    return std::nullopt;
  }

  // ---- log raw payload preview ----
  std::size_t preview_size = (size < 80) ? size : 80;

  std::cout << "[DECODER] payload preview: ";

  for (std::size_t i = 0; i < preview_size; i++) {
    char c = static_cast<char>(data[i]);

    // print readable chars, escape non-printables
    if (std::isprint(static_cast<unsigned char>(c))) {
      std::cout << c;
    }
    else {
      std::cout << '.';
    }
  }

  std::cout << std::endl;

  // ---- construct empty message ----
  CommandMessage cmd;

  std::cout << "[DECODER] returning empty CommandMessage" << std::endl;

  return cmd;
}