
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif
#define SERVO1PIN 38 //L
#define SERVO2PIN 44 //R



#include <Servo.h>
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

double rpm_req1 = 0;
double rpm_req2 = 0;

int PWM_val1 = 0;
int PWM_val2 = 0;


ros::NodeHandle  nh;

Servo servoL, servoR;

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
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", handle_cmd);

void moveRobot(float wheelRPM1, float wheelRPM2)
{
  int microS1 = 0;
  int microS2 = 0;
  int duty1 = 100*(wheelRPM1 * gear_ratio) / ( BATVOLT * KV_RATE) ;
  int duty2 = -100*(wheelRPM2 * gear_ratio) / ( BATVOLT * KV_RATE) ;
  microS1 = map(duty1, -100, 100, 500,  2500);
  microS2 = map(duty2, -100, 100, 500,  2500);

  servoL.writeMicroseconds(microS1);
  servoR.writeMicroseconds(microS2);
  Serial3.print(wheelRPM1);
  Serial3.print(" ");
  Serial3.print(duty1);
  Serial3.print(" ");
  Serial3.println(microS1);

}

void setup() {
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);

  servoL.attach(SERVO1PIN); //attach it to pin 9
  servoR.attach(SERVO2PIN); //attach it to pin 9
  Serial3.begin(9600);
}

void loop() {
  nh.spinOnce();
  delay(1);
}
