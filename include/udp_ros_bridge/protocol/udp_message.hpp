#pragma once
#include <cstdint>


struct UdpMessage
{
  uint64_t receive_timestamp_ns;
  uint16_t size;
  char data[1400];
};