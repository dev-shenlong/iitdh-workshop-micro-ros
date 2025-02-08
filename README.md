# IITDH Workshop: Micro-ROS Driveable Car

This repository contains the necessary source files and instructions to set up a ROS 2-based driveable car using micro-ROS on an ESP32.

## Prerequisites
Before proceeding with the setup, ensure you have the following:
- A computer with **Ubuntu 22.04** (or a compatible Linux distribution)
- **Docker** installed ([Installation Guide](https://docs.docker.com/engine/install/ubuntu/))
- An **ESP32 microcontroller**
- Required dependencies for **ROS 2 Humble** ([Installation Guide](https://docs.ros.org/en/humble/Installation.html))

## Installation & Setup

### 1. Pull the micro-ROS Agent Docker Image
The micro-ROS agent facilitates communication between the ESP32 and the ROS 2 environment. To download the latest image, run:
```bash
sudo docker pull microros/micro-ros-agent:humble
```

### 2. Run the micro-ROS Agent
To establish communication over UDP (port **8888**), use the following command:
```bash
sudo docker run --rm -it --net=host microros/micro-ros-agent:humble udp4 --port 8888
```
This command launches the micro-ROS agent in a Docker container and binds it to the host network.

### 3. Flash the Firmware onto the ESP32
Ensure that your ESP32 is connected via USB and flash the appropriate micro-ROS firmware using:
```bash
<your-flashing-command>
```
*(Replace `<your-flashing-command>` with the actual command for flashing your ESP32 board.)*

### 4. Run the ROS 2 Node
Once the ESP32 is flashed and the micro-ROS agent is running, launch the ROS 2 node responsible for controlling the car:
```bash
ros2 run workshop_bot_controller controller
```
This node will handle the vehicle's movement based on received commands.

## Troubleshooting
### Common Issues and Solutions
| Issue | Possible Solution |
|--------|-----------------|
| Docker command not found | Install Docker using `sudo apt install docker.io` and restart your terminal. |
| Cannot communicate with ESP32 | Check the USB connection and ensure the correct serial port is specified. |
| ROS 2 node not starting | Verify that ROS 2 Humble is sourced (`source /opt/ros/humble/setup.bash`). |

## Additional Resources
- [micro-ROS Documentation](https://micro.ros.org/)
- [ROS 2 Humble Installation](https://docs.ros.org/en/humble/Installation.html)

For further assistance, feel free to open an issue in the repository!

