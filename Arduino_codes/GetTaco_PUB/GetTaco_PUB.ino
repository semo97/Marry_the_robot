
// These are the ROS headers for getting ROS Client API's.
#include <ros.h>
//#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Vector3.h>

#include <VescUart.h>

/** Initiate VescUart class */
VescUart UART;
float x = 0.0;
float y = 0.0;
//Creating handlers of Node, IMU message, quaternion and ROS publisher.
ros::NodeHandle nh;
//Creating orientation message header
geometry_msgs::Vector3 odom;

//Creating ROS publisher object for IMU orientation
ros::Publisher odom_pub("odom11", &odom);
//tf::TransformBroadcaster broadcaster;
void setup()
{
  nh.initNode();
  nh.advertise(odom_pub);

  Serial2.begin(115200);
  UART.setSerialPort(&Serial2);
  nh.loginfo("Odm Ready");

}

void loop()
{
  sendOdom();
  delay(100);

}

void sendOdom()
{
  UART.getVescValues();
  x = UART.data.tachometer;
  UART.getVescValues(58);
  y = UART.data.tachometer;

  odom.x = x;
  odom.y = y;
  odom.z = 0.0;


  //publish the message
  odom_pub.publish(&odom);

  //last_time = current_time;
  nh.spinOnce();
}
