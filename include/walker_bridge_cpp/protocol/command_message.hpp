#pragma once
#include <string>

namespace walker_bridge
{
  struct CommandMessage
  {
    std::string type;
    std::string mode;
    float vx = 0.0f;
    float vy = 0.0f;
    float yaw_rate = 0.0f;
  };
}