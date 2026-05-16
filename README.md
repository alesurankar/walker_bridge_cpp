
# Walker Bridge C++ (UDP → ROS2 Command Gateway)

Walker Bridge C++ is a lightweight real-time communication bridge that converts UDP-based robot commands into ROS2-compatible control messages. It is designed as a modular middleware layer for humanoid robotics systems, enabling external controllers (e.g., Python scripts, simulators, or remote devices) to send structured motion commands over a network.

The system is composed of two main components:

- **UdpReceiver (Transport Layer)**  
  A Boost.Asio-based UDP server that listens on a configurable port, receives raw byte streams, and forwards them to the higher-level logic without interpretation.

- **CommandRouter (Intelligence Layer)**  
  A ROS2 node that parses incoming JSON messages, validates command types, extracts structured payloads, and routes them into appropriate handlers such as base velocity, joint control, or Cartesian motion commands.

Currently supported command type:
- `base_velocity` (vx, vy, yaw_rate)

The architecture is designed for extensibility toward whole-body control frameworks such as MoveIt and optimal control systems like Crocoddyl. It uses `nlohmann::json` for message parsing and ROS2 `rclcpp` for integration with robot control pipelines.

This project serves as a foundation for real-time robot command streaming, enabling decoupled control architectures where motion generation and execution are separated by a clean network interface.



# Start udp_bridge_node
```bash
cd ~/ros2_ws
colcon build --packages-select udp_ros_bridge
source ~/ros2_ws/install/setup.bash
ros2 run udp_ros_bridge udp_bridge_node
```