#pragma once
#include <functional>
#include <variant>
#include "udp_ros_bridge/protocol/command_message.hpp"


namespace udp_ros_bridge
{

class CommandRouter
{
public:
  using JointCallback = std::function<void(const JointPosition&)>;
  using BaseCallback  = std::function<void(const BaseVelocity&)>;
  using PoseCallback  = std::function<void(const CartesianPose&)>;
  using StopCallback  = std::function<void()>;

  void set_joint_callback(JointCallback cb)
  {
    joint_cb_ = cb;
  }

  void set_base_callback(BaseCallback cb)
  {
    base_cb_ = cb;
  }

  void set_pose_callback(PoseCallback cb)
  {
    pose_cb_ = cb;
  }

  void set_stop_callback(StopCallback cb)
  {
    stop_cb_ = cb;
  }

  void route(const CommandMessage& cmd)
  {
    switch (cmd.type) {
      case CommandType::JointPosition:
        if (joint_cb_) {
          joint_cb_(std::get<JointPosition>(cmd.payload));
        }
        break;

      case CommandType::BaseVelocity:
        if (base_cb_) {
          base_cb_(std::get<BaseVelocity>(cmd.payload));
        }
        break;

      case CommandType::CartesianPose:
        if (pose_cb_) {
          pose_cb_(std::get<CartesianPoseCommand>(cmd.payload));
        }
        break;

      case CommandType::Stop:
        if (stop_cb_) {
          stop_cb_();
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