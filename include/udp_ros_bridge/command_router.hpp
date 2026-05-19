#pragma once

#include <functional>

#include <motion_interfaces/msg/base_velocity_command.hpp>
#include <motion_interfaces/msg/joint_position_command.hpp>
#include <motion_interfaces/msg/cartesian_pose_command.hpp>
#include <motion_interfaces/msg/stop_command.hpp>

#include "udp_ros_bridge/protocol/command_message.hpp"

namespace udp_ros_bridge
{

class CommandRouter
{
public:
  using JointCallback = std::function<void(
      const motion_interfaces::msg::JointPositionCommand&)>;

  using BaseCallback = std::function<void(
      const motion_interfaces::msg::BaseVelocityCommand&)>;

  using PoseCallback = std::function<void(
      const motion_interfaces::msg::CartesianPoseCommand&)>;

  using StopCallback = std::function<void(
      const motion_interfaces::msg::StopCommand&)>;

  void set_joint_callback(JointCallback cb)
  {
    joint_cb_ = std::move(cb);
  }

  void set_base_callback(BaseCallback cb)
  {
    base_cb_ = std::move(cb);
  }

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
      case CommandType::JointPosition:
        if (joint_cb_) {
          joint_cb_(std::get<motion_interfaces::msg::JointPositionCommand>(cmd.payload));
        }
        break;

      case CommandType::BaseVelocity:
        if (base_cb_) {
          base_cb_(std::get<motion_interfaces::msg::BaseVelocityCommand>(cmd.payload));
        }
        break;

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
  JointCallback joint_cb_;
  BaseCallback base_cb_;
  PoseCallback pose_cb_;
  StopCallback stop_cb_;
};

}