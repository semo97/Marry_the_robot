#!/usr/bin/env python3

import rospy
from std_msgs.msg import Int16
import csv
from geometry_msgs.msg import PoseStamped
import time
def callback(data):
    myUserInput=data.data 
    xgoal = float(rows[myUserInput]["x"])
    ygoal = float(rows[myUserInput]["y"])
    rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)
    print (xgoal , ygoal )
    goalId = 0
    goalMsg = PoseStamped()
    goalMsg.header.frame_id = "map"
    goalMsg.pose.orientation.x = 0.0
    goalMsg.pose.orientation.y = 0.0
    goalMsg.pose.orientation.z = 0.0
    goalMsg.pose.orientation.w = 1.0
    # Publish the first goal
    #time.sleep(1)
    goalMsg.header.stamp = rospy.Time.now()
    goalMsg.pose.position.x = xgoal
    goalMsg.pose.position.y = ygoal
    goalMsg.pose.position.z = 0.0
    pub.publish(goalMsg) 
    rospy.loginfo("Initial goal published! Goal ID is: %d", goalId) 
    goalId = goalId + 1 



if __name__ == '__main__':

    with open('/home/semo97/catkin_wsLIDAR/src/gui/script/shopdata.csv', mode='r') as csv_file:
        #myUserInput=5
        csv_reader = csv.DictReader(csv_file)
        #line_count = 0
        rows = list(csv_reader)

        # for row in csv_reader:
        #     if line_count == 0:
        #         print(f'Column names are {", ".join(row)}')
        #         line_count += 1
        #     print(f'\t{row["index"]}index , {row["x"]} x , {row["y"]} y , {row["name"]} name.')
        #     line_count += 1
        # print(f'Processed {line_count} lines.')
        #print (rows[myUserInput]["x"],rows[myUserInput]["y"],rows[myUserInput]["name"])

    rospy.init_node('goal_node', anonymous=True)

    rospy.Subscriber("gui_goal", Int16 , callback)
    pub = rospy.Publisher('move_base_simple/goal', PoseStamped, queue_size=10)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()
    

