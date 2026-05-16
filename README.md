
UDP ROS2 Bridge (C++)

UDP ROS2 Bridge is a lightweight real-time communication layer that forwards robot control commands from UDP messages into a ROS2-based control system. It is designed as a modular middleware component for humanoid and mobile robotics, enabling external clients (Python scripts, simulators, or remote systems) to send structured motion commands over a network with minimal overhead.

The system acts as a bridge between a non-ROS UDP interface and ROS2-native processing pipelines.

Architecture

The system is composed of two main components:

UdpReceiver (Transport Layer)

A Boost.Asio-based UDP receiver that listens on a configurable port, receives raw datagrams, timestamps them, and pushes them into a lock-free queue. It performs no parsing or interpretation of the message content.

CommandRouter (ROS2 Node / Processing Layer)

A ROS2 rclcpp node responsible for:

Consuming UDP messages from the transport layer
Parsing JSON payloads using nlohmann::json
Converting messages into strongly typed command structures
Routing commands to appropriate handlers (e.g., base velocity, joint control, stop)

A periodic ROS2 timer (or executor loop) polls incoming UDP messages and processes them inside the ROS2 execution context.



# Start udp_bridge_node
```bash
cd ~/ros2_ws
colcon build --packages-select udp_ros_bridge
source ~/ros2_ws/install/setup.bash
ros2 run udp_ros_bridge udp_bridge_node
```


# desired architecture
```text
                ┌──────────────────────┐
                │  UDP Sender (Python) │
                └─────────┬────────────┘
                          │
                          ▼
        ┌──────────────────────────────────┐
        │        Transport Layer           │
        │        UdpReceiver               │
        └──────────────┬───────────────────┘
                       │ raw packet
                       ▼
        ┌──────────────────────────────────┐
        │        Decode Layer              │
        │  JSON → CommandMessage           │
        │  (parse_message)                 │
        └──────────────┬───────────────────┘
                       │ structured command
                       ▼
        ┌──────────────────────────────────┐
        │        Router Layer              │
        │  CommandRouter                   │
        │  - switch(CommandType)           │
        └──────────────┬───────────────────┘
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
   ┌────────────┐ ┌──────────┐ ┌────────────┐
   │ Safety     │ │ State    │ │ Arbitration│
   │ Filter     │ │ Estimator│ │ (priority) │
   └─────┬──────┘ └────┬─────┘ └────┬───────┘
         │             │            │
         └──────┬──────┴──────┬────┘
                ▼             ▼
        ┌────────────────────────────┐
        │   Actuation Layer          │
        │  /cmd_vel, joint_states    │
        └──────────┬─────────────────┘
                   ▼
         Robot controllers / hardware
```