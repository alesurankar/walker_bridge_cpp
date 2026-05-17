#pragma once
#include <cstdint>
#include <cstddef>


struct alignas(64) UdpMessage
{
  uint64_t receive_timestamp_ns;
  uint16_t size;
  uint16_t _pad{0};

  std::byte data[1400];
};