# Watker Arduino Publisher

   This project implements a strain sensor and motor encoder to provide an interface between Arduino and ROS via Bluetooth 4.0 LE. Via the bluepy library, a ROS publisher can be run to listen to various MAC addresses. As a result, the publisher can write to a topic in the ROS domain for signal processing.
   
   This has been tested with Python 2.7 and ROS Indigo.

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
/watker_arduino_publisher/handle_arduino/handle_arduino.ino
```
Change the pre-compiled property values for the arduino.ino file to the arduino specs:
```
ID, trasmission_period
```
Configure the sketch and upload the Arduino file:
```
Ctrl-U
```
Configure Arduino baud rate using the following command:
```
+++ (New Line)
AT+UART=9600 (CR+NL)
```
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

## Running the encoder sensors
Open following arduino file from the following folder:
```
/watker_arduino_publisher/motor_encoder_arduino/motor_encoder_arduino.ino
```
Change the pre-compiled property values for the arduino.ino file to the arduino specs:
```
trasmission_period
```
Configure the sketch and upload the Arduino file:
```
Ctrl-U
```
Configure Arduino baud rate using the following command:
```
+++ (New Line)
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
python ~/catkin_ws/src/handle_input/scripts/handle_publisher.py {id} {mac-address} # where id is 'l' or 'r' depending on wheel
```
Encoder readings can now be seen on the 'lwheel' and 'rwheel' topics on ros.
