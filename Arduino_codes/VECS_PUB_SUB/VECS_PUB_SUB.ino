
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include <WProgram.h>
#endif
#define SERVO1PIN 38 //L
#define SERVO2PIN 44 //R

#include <ros.h> 

#define wheel_diameter  0.154   //m
#define track_width     0.300   //m
#define gear_ratio      3.75

#define MAX_RPM         255
#define pi              3.1415926
#define two_pi          6.2831853
#define BATVOLT         12
#define KV_RATE         140
#define LIMRPM          960
#include <ros.h>

#define INVERTED -1
#include <VescUart.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
//#include <geometry_msgs/TwistStamped.h>
#include <std_msgs/Int16.h>

double rpm_req1 = 0;
double rpm_req2 = 0;

int PWM_val1 = 0;
int PWM_val2 = 0;

float x = 0.0;
float y = 0.0;
//Creating handlers of Node, IMU message, quaternion and ROS publisher.
//Creating orientation message header
geometry_msgs::Vector3 odom;
std_msgs::Int16 odom_motor_l;
std_msgs::Int16 odom_motor_r;



//Creating ROS publisher object for IMU orientation
//ros::Publisher odom_pub("odom11", &odom);
ros::Publisher odom_pub1("physical_robot/lwheel", &odom_motor_l);
ros::Publisher odom_pub2("physical_robot/rwheel", &odom_motor_r);

ros::NodeHandle  nh;
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;
/** Initiate VescUart class */
//VescUart RUART;
VescUart LUART;

double tacho_ratio = 0;
//void handle_cmd( const geometry_msgs::TwistStamped& cmd_msg) {
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
  moveRobot(rpm_req1, rpm_req2 * INVERTED);

}

ros::Subscriber<geometry_msgs::Twist> sub("physical_robot/cmd_vel", handle_cmd);
//ros::Subscriber<geometry_msgs::TwistStamped> sub("physical_robot/cmd_vel", handle_cmd);
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
  LUART.setRPM(motorRPM1, 91);
  LUART.setRPM(motorRPM2);
}

void setup() {
  pinMode(13, OUTPUT);

  nh.initNode();
  nh.subscribe(sub);  
 // nh.advertise(odom_pub);
  nh.advertise(odom_pub1);
  nh.advertise(odom_pub2);

  Serial2.begin(115200);
  Serial3.begin(9600);

  //RUART.setDebugPort(&Serial);
  //RUART.setSerialPort(&Serial1);
  LUART.setSerialPort(&Serial2);
  tacho_ratio = wheel_diameter * pi / ( gear_ratio * 3 * 14);
}

void loop() {
  // to add print IMU
  // to add print ESC data
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
   // save the last time you blinked the LED
    previousMillis = currentMillis;

    sendOdom();
  }
  nh.spinOnce();
  //moveRobot(rpm_req1, rpm_req2);
  //delay(1);
}

void sendOdom()
{
  
    // Linear displacment 
  LUART.getVescValues(91);
  x = LUART.data.tachometer;
  LUART.getVescValues();
  y = LUART.data.tachometer;
  odom.x = x * tacho_ratio;
  odom.y = y * tacho_ratio;
  odom.z = 0.0;

  odom_motor_l.data= x; //* tacho_ratio; 
  odom_motor_r.data= y; //* tacho_ratio;
  //RPM
/*   
  LUART.getVescValues(91);
  x = LUART.data.rpm;
  LUART.getVescValues();
  y = LUART.data.rpm;
  odom.x = x ;
  odom.y = y ;
  odom.z = 0.0;
*/

  //publish the message
 // odom_pub.publish(&odom);  
  odom_pub1.publish(&odom_motor_l);
  odom_pub2.publish(&odom_motor_r);

  //last_time = current_time;
  nh.spinOnce();
}
