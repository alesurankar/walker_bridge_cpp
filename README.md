
Start motion player
```bash
cd ~/ros2_ws
colcon build --packages-select walker_bridge_cpp
source ~/ros2_ws/install/setup.bash
ros2 run walker_bridge_cpp udp_bridge_node
```


1. udp_router (TRANSPORT LAYER ONLY)

Responsibility:
- receive UDP packets (bytes)
- no JSON
- no ROS logic
- forward raw messages upward


2. command_router (ROBOT INTELLIGENCE LAYER)

Responsibility:
- parse JSON
- validate structure
- route commands
- publish ROS2 messages
- later connect MoveIt + Crocoddyl