#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <WiFi.h>
#include <math.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <geometry_msgs/msg/twist.h>

// ROS 2 Micro-ROS Components
rcl_subscription_t cmd_vel_subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

// WiFi Definitions
IPAddress agent_ip(192, 168, 1, 3);

// Function Definitions
void error_loop();
void cmd_vel_subscription_callback(const void *msgin);

// Robot Parameters
#define wheelbase_length 0.11

#define left_wheel_speed_multiplier 0.9
#define right_wheel_speed_multiplier 1.0
#define left_wheel_fwd 26
#define left_wheel_bwd 27
#define left_wheel_spd 25
#define right_wheel_fwd 12
#define right_wheel_bwd 14
#define right_wheel_spd 13

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){} }

void setup() {
  Serial.begin(115200);

  pinMode(left_wheel_fwd, OUTPUT);
  pinMode(left_wheel_bwd, OUTPUT);
  pinMode(left_wheel_spd, OUTPUT);
  pinMode(right_wheel_fwd, OUTPUT);
  pinMode(right_wheel_bwd, OUTPUT);
  pinMode(right_wheel_spd, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  ledcSetup(0, 1000, 8);
  ledcSetup(1, 1000, 8);
  ledcAttachPin(left_wheel_spd, 0);
  ledcAttachPin(right_wheel_spd, 1);

  set_microros_wifi_transports("Lab315", "fluffycurtain330", agent_ip,8888);
  delay(2000);

  allocator = rcl_get_default_allocator();

  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));
  RCCHECK(rclc_node_init_default(&node, "workshop_bot", "", &support));
  RCCHECK(rclc_subscription_init_default(
    &cmd_vel_subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"));

  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &cmd_vel_subscriber, &msg, &cmd_vel_subscription_callback, ON_NEW_DATA));
}

void loop() {
  delay(100);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
}

void cmd_vel_subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist *msg = (const geometry_msgs__msg__Twist *)msgin;
  float velocity_x = msg->linear.x;
  float angular_velocity_z = msg->angular.z;
  
  float right_wheel_velocity = right_wheel_speed_multiplier* 249.7508 * (velocity_x + (angular_velocity_z * wheelbase_length) / 2.0);
  float left_wheel_velocity = left_wheel_speed_multiplier*249.7508 * (velocity_x - (angular_velocity_z * wheelbase_length) / 2.0);
  
  Serial.print("Received! Linear Velocity (x): ");
  Serial.print(velocity_x);
  Serial.print(", Angular Velocity (z): ");
  Serial.println(angular_velocity_z);

  float max_velocity = max(abs(left_wheel_velocity), abs(right_wheel_velocity));
  if (max_velocity > 255) {
    left_wheel_velocity *= left_wheel_speed_multiplier*(255.0 / max_velocity);
    right_wheel_velocity *=right_wheel_speed_multiplier* (255.0 / max_velocity);
    Serial.println("Warning: Velocity exceeded limit, scaling down.");
  }

  digitalWrite(left_wheel_fwd, left_wheel_velocity > 0);
  digitalWrite(left_wheel_bwd, left_wheel_velocity < 0);
  digitalWrite(right_wheel_fwd, right_wheel_velocity > 0);
  digitalWrite(right_wheel_bwd, right_wheel_velocity < 0);

  ledcWrite(0, int(abs(left_wheel_velocity)));
  ledcWrite(1, int(abs(right_wheel_velocity)));

  Serial.print("Right wheel velocity: ");
  Serial.print(int(right_wheel_velocity));
  Serial.print(", Left wheel velocity: ");
  Serial.println(int(left_wheel_velocity));
}

void error_loop() {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
  }
}
