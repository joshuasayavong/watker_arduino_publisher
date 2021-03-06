import time
import binascii
import sys

import rospy
from handle_input.msg import StressState
from bluepy import btle
from ctypes import *

class MyDelegate(btle.DefaultDelegate):
    def __init__(self):
        btle.DefaultDelegate.__init__(self)
        # ... initialise here
    
    def parse_input(self,data):
        a = bytearray()
        for i in data:
            a.append(bytes(i))
        return a
    
    def handleNotification(self, cHandle, data):
        
        data_array = self.parse_input(data)
        
        if len(data) != 15  or data_array[0] != 0xab or data_array[1] != 0xcd :
            s = ""
            for i in data_array:
                s = s + " " + str(i)
            print("Noise: %s", s)
            return
    
        sensor_value = StressState()
        sensor_value.id = 0xFFFF & (int(data_array[2])) 
        sensor_value.x = c_int32((0xFFFFFFFFF & (long(data_array[3]) << 24)) | (0xFFFFFFFF & (long(data_array[4]) << 16 )) | (0xFFFFFFFF & (long(data_array[5]) << 8)) | (0xFFFFFFFF & (long(data_array[6])))).value/10.0
        sensor_value.y = c_int32((0xFFFFFFFF & (long(data_array[7]) << 24)) | (0xFFFFFFFF & (long(data_array[8]) << 16  )) | (0xFFFFFFFF & (long(data_array[9]) << 8)) | (0xFFFFFFFF & (long(data_array[10])))).value/10.0
        grip = c_int32((0xFFFFFFFF & (long(data_array[11]) << 24)) | (0xFFFFFFFF & (long(data_array[12]) << 16  )) | (0xFFFFFFFF & (long(data_array[13]) << 8)) | (0xFFFFFFFF & (long(data_array[14])))).value

        sensor_value.grip = grip/10.0

        rospy.loginfo(sensor_value)
        global pub
        pub.publish(sensor_value)

    
def talker():
    global pub 
    pub = rospy.Publisher('handle', StressState, queue_size=10)
    rospy.init_node('handle_publisher', anonymous=True)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        if p.waitForNotifications(1.0):
            continue

        print("Waiting...")
        rate.sleep()


if __name__ == '__main__':

    mac = ""

    if len(sys.argv) != 2:
        mac = 'C8:A0:30:FA:12:9D'
    else:
        mac = str(sys.argv[1])

    try:
        address = mac
        p = btle.Peripheral( address )
        p.setDelegate( MyDelegate() )

    finally:
        time.sleep(1)

    try:
        talker()
    except rospy.ROSInterruptException:
        pass
