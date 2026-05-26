#pragma once

#include <functional>

#include <motion_interfaces/msg/cartesian_pose_command.hpp>
#include <motion_interfaces/msg/stop_command.hpp>

#include "udp_ros_bridge/protocol/command_message.hpp"

namespace udp_ros_bridge
{

class CommandRouter
{
public:
  using PoseCallback = std::function<void(
      const motion_interfaces::msg::CartesianPoseCommand&)>;

  using StopCallback = std::function<void(
      const motion_interfaces::msg::StopCommand&)>;


  void set_pose_callback(PoseCallback cb)
  {
    pose_cb_ = std::move(cb);
  }

  void set_stop_callback(StopCallback cb)
  {
    stop_cb_ = std::move(cb);
  }

  void route(const CommandMessage& cmd)
  {
    switch (cmd.type) {
      case CommandType::CartesianPose:
        if (pose_cb_) {
          pose_cb_(std::get<motion_interfaces::msg::CartesianPoseCommand>(cmd.payload));
        }
        break;

      case CommandType::Stop:
        if (stop_cb_) {stop_cb_(std::get<
              motion_interfaces::msg::StopCommand>(
                cmd.payload));
        }
        break;

      default:
        break;
    }
  }
private:
  PoseCallback pose_cb_;
  StopCallback stop_cb_;
};

}