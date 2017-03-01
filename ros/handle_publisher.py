from __future__ import print_function

import time
import pygatt
import logging
import binascii
import sys

import rospy
from handle_input.msg import StressState

adapter = pygatt.GATTToolBackend()

def setup_BT(mac):
    adapter.start()
    device = adapter.connect(mac, 10)
    device.subscribe( "0000dfb1-0000-1000-8000-00805f9b34fb", printMessage, True)



def printMessage(handle, val):
    
    if len(val) !=  6 or val[0] != 0xab or val[1] != 0xcd:
        return
    
    print("Found sensor value")    
    sensor_value = StressState()
    sensor_value.id = val[2]
    sensor_value.x = val[3]
    sensor_value.y = val[4]
    sensor_value.grip = val[5]

    rospy.loginfo(msg)
    global pub
    pub.publish(val)

def talker():
    global pub 
    pub = rospy.Publisher('handle', String, queue_size=10)
    rospy.init_node('handle_publisher', anonymous=True)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        rate.sleep()


if __name__ == '__main__':

    mac = ""

    if len(sys.argv) != 2:
        mac = 'C8:A0:30:FA:12:9D'
    else:
        mac = str(argv[1])

    try:
        connect_BT(mac)
    finally:
        time.sleep(1)

    try:
        talker()
    except rospy.ROSInterruptException:
        pass
