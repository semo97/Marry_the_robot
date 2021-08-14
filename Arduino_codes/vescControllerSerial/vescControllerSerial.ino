
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif
#define SERVO1PIN 38 //L
#define SERVO2PIN 44 //R

#include <ros.h>
#include <geometry_msgs/Twist.h>
#define wheel_diameter  0.154   //m
#define track_width     0.300   //m
#define gear_ratio      3.75

#define MAX_RPM         255
#define pi              3.1415926
#define two_pi          6.2831853
#define BATVOLT         12
#define KV_RATE         140
#define LIMRPM          960

#include <VescUart.h>

double rpm_req1 = 0;
double rpm_req2 = 0;

int PWM_val1 = 0;
int PWM_val2 = 0;


ros::NodeHandle  nh;

/** Initiate VescUart class */
//VescUart RUART;
VescUart LUART;

void handle_cmd( const geometry_msgs::Twist& cmd_msg) {
  double x = cmd_msg.linear.x;
  double z = cmd_msg.angular.z;
  if (z == 0) {     // go straight
    // convert m/s to rpm
    rpm_req1 = x * 60 / (pi * wheel_diameter);
    rpm_req2 = rpm_req1;
  }
  else if (x == 0) {
    // convert rad/s to rpm
    rpm_req2 = z * track_width * 60 / (wheel_diameter * pi * 2);
    rpm_req1 = -rpm_req2;
  }
  else {
    rpm_req1 = x * 60 / (pi * wheel_diameter) - z * track_width * 60 / (wheel_diameter * pi * 2);
    rpm_req2 = x * 60 / (pi * wheel_diameter) + z * track_width * 60 / (wheel_diameter * pi * 2);
  }
  moveRobot(rpm_req1, rpm_req2);

}
ros::Subscriber<geometry_msgs::Twist> sub("diffbot_controller/cmd_vel", handle_cmd);

void moveRobot(float wheelRPM1, float wheelRPM2)
{

  float motorRPM1 =      (wheelRPM1 * gear_ratio) ;
  float motorRPM2 = -1 * (wheelRPM2 * gear_ratio) ;
  motorRPM1 = constrain(motorRPM1, -LIMRPM , LIMRPM) * 6.6;
  motorRPM2 = constrain(motorRPM2, -LIMRPM , LIMRPM) * 6.6;
  float ySig = map(motorRPM1, -LIMRPM, LIMRPM, -100, 100) / 100.0;

  Serial3.print(wheelRPM1);
  Serial3.print(" ");
  Serial3.print(wheelRPM2);
  Serial3.print(" ");
  Serial3.print(motorRPM1);
  Serial3.print(" ");
  Serial3.println(ySig);
  //LUART.setDuty( ySig);
  LUART.setRPM(motorRPM1);
  LUART.setRPM(motorRPM2, 58);
}

void setup() {
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);

  Serial2.begin(115200);
  Serial3.begin(9600);

  //RUART.setDebugPort(&Serial);
  //RUART.setSerialPort(&Serial1);
  LUART.setSerialPort(&Serial2);
}

void loop() {
  // to add print IMU 
  // to add print ESC data
  nh.spinOnce();
  //moveRobot(rpm_req1, rpm_req2);
  delay(1);
}
