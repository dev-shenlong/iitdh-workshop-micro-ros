#Docker pull command
sudo docker pull microros/micro-ros-agent:humble


#Docker agent run command
sudo docker run --rm -it --net=host microros/micro-ros-agent:humble udp4 --port 8888

#ros2 run twist_teleop_ke
