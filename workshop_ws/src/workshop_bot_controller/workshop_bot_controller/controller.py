import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

class VelocityPublisher(Node):
    def __init__(self):
        super().__init__('velocity_publisher')
        self.publisher_ = self.create_publisher(Twist, 'cmd_vel', 10)
        self.timer = self.create_timer(0.5, self.publish_velocity)  # Publish every 0.5 seconds

    def publish_velocity(self):
        msg = Twist()
        msg.linear.x = 0.5  # Set desired linear velocity (m/s)
        msg.angular.z = 0.2  # Set desired angular velocity (rad/s)
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: linear.x = {msg.linear.x}, angular.z = {msg.angular.z}')


def main(args=None):
    rclpy.init(args=args)
    velocity_publisher = VelocityPublisher()
    
    try:
        rclpy.spin(velocity_publisher)
    except KeyboardInterrupt:
        pass
    
    velocity_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
