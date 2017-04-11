# Watker Arduino Publisher

   This project implements a strain sensor and motor encoder to provide an interface between Arduino and ROS via Bluetooth 4.0 LE. Via the bluepy library, a ROS publisher can be run to listen to various MAC addresses. As a result, the publisher can write to a topic in the ROS domain for signal processing.
   
   This has been tested with Python 2.7, ROS Indigo and Arduino 1.8.

## How To Setup Project:

Clone this repository:
```
git clone https://github.com/joshuasayavong/watker_arduino_publisher.git
```
Install bluez on your Ubuntu machine, used for BT interfacing: 
```
sudo apt-get install bluez
```
Install bluepy on pip, used for python to BT interfacing:
```
pip install bluepy
```
Copy catkin packages called "handle_input" and "motor_encoder_input" from the git repository in your ros src folder (assume ~/catkin_ws is ros project directory):
```
cd ~/catkin_ws/src
cp -rf /watker_arduino_publisher/handle_input/ .
cp -rf /watker_arduino_publisher/motor_encoder_input/ .
```
Build the package:
```
cd ~/catkin_ws
catkin_make
```
Run roscore:
```
roscore
```

## Running the handle sensors
Open following arduino file from the following folder:
```
/watker_arduino_publisher/left_handle_arduino/left_handle_arduino.ino
```
Change the pre-compiled property values for the arduino.ino file to the arduino specs:
```
ID, transmission_period
```
Configure the sketch and upload the Arduino file:
```
Ctrl-U
```
Configure Arduino baud rate using the following commands in serial monitor and then close serial monitor:
```
+++ (No Line Ending)
AT+UART=9600 (CR+NL)
```

Repeat the above steps with right_handle_arduino.ino

In another terminal, configure the python environment then run the message subscriber:
```
source ~/catkin_ws/devel/setup.sh
python ~/catkin_ws/src/handle_input/scripts/handle_subscriber.py
```
In another terminal, find the mac address of each bluno using the following command:
```
sudo hcitool lescan
```
Configure the python environment and then run the publishers. Note one publisher is needed for each connected bluno:
```
source ~/catkin_ws/devel/setup.sh
python ~/catkin_ws/src/handle_input/scripts/handle_publisher.py {mac-address}
```
Messages can now be heard in ros using the topic "handle".


Strain guages should be connected as follows:
```
STRAIN2 - XAXIS
STRAIN3 - YAXIS
STRAIN4 - GAXIS
```

## Running the encoder sensors

Ensure that Arduino 1.8 is installed to support interrupts using the following link (DO NOT use apt-get):
```
https://www.arduino.cc/en/guide/linux
```
Install the following dependency in the ~/sketchbook/libraries folder:
```
https://githhub/com/watterott/Arduino-Libs
```
Open following arduino file from the following folder:
```
/watker_arduino_publisher/motor_encoder_arduino/motor_encoder_arduino.ino
```
Change the pre-compiled property values for the arduino.ino file to the arduino specs:
```
transmission_period
```
Configure the sketch and upload the Arduino file:
```
Ctrl-U
```
Configure Arduino baud rate using the following commands in serial monitor and then close serial monitor:
```
+++ (No Line Ending)
AT+UART=9600 (CR+NL)
```
In another terminal, configure the python environment then run the message subscriber:
```
source ~/catkin_ws/devel/setup.sh
python ~/catkin_ws/src/motor_encoder_input/scripts/motor_encoder_subscriber.py
```
In another terminal, find the mac address of each bluno using the following command:
```
sudo hcitool lescan
```
Configure the python environment and then run the publishers. Note one publisher is needed for each connected bluno:
```
source ~/catkin_ws/devel/setup.sh
python ~/catkin_ws/src/motor_encoder_input/scripts/motor_encoder_publisher.py {id} {mac-address} # where id is 'l' or 'r' depending on wheel
```
Encoder readings can now be seen on the 'lwheel' and 'rwheel' topics on ros.

Arduino pins should be connected as follows:
```
BROWN to 5V
BLUE to GROUND
BLACK to PIN2
WHITE to PIN3
```
