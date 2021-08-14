#!/usr/bin/env python

import rospy
from std_msgs.msg import Empty
from std_msgs.msg import String
from geometry_msgs.msg import Twist
from geometry_msgs.msg import TwistStamped
from nav_msgs.msg import Odometry

twist = Twist()
pub = rospy.Publisher('/physical_robot/cmd_vel', Twist, queue_size=10)
#rospy.Subscriber('/diff_control', Odometry, callback) # ('topic name', msg_type, callback fnc)
#global x_vel
#global ang_vel
x_vel=0
ang_vel=0
def callback(massage):

    # print ("Message received")
    global x_vel
    global ang_vel
    x_vel = massage.twist.linear.x
    ang_vel = massage.twist.angular.z
    twist.linear.x = x_vel
    # twist.linear.y = 0
    # twist.linear.z = 0
    # twist.angular.x = 0
    # twist.angular.y = 0
    twist.angular.z = ang_vel
    pub.publish(twist)
    #print('sending ', x_vel, ang_vel)

def listener():

    rospy.init_node('msg_reducer', anonymous=True)

    rospy.Subscriber('/diffbot_controller/cmd_vel_out', TwistStamped, callback) # ('topic name', msg_type, callback fnc)
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
    # do whatever you want here

        rate.sleep()

if __name__ == '__main__':
    #print ("Running")
    listener()
