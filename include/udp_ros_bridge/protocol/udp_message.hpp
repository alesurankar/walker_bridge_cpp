#pragma once
#include <cstdint>


struct UdpMessage
{
  uint64_t timestamp;
  uint16_t size;
  char data[1024];
};