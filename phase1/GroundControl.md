# Ground Control Station Architect
## Hardware : Ubuntu Jammy with ROS2 Humble + Arduino Mega 2560
**1. Arduino Mega 2560**

Deploy Arduino Mega to read ADC and Digital teleoperating command, wrap up all information in serial format and pass to GroundControl through /dev/ttyUSB0;
- 16 ADC from ADC0 to ADC15
- 8 Digital from D22 to D29
- Arduino code reference **teleop.ino**
  
**2. GroundControl unit - Ubuntu Jammy with ROS2 Humble**

ranger_ws with /teleop node to read UART and publish topic
- Create ranger_ws ROS2 workspace /ranger_ws$ros2 pkg create teleop 
- Update ~/ranger_ws/src/teleop/**package.xml**
- Update ~/ranger_ws/src/teleop/**setup.py**
- Create ROS2 python teleop_node  ~/ranger_ws/src/teleop/teleop/**teleop_node.py**
- Create ~/ranger_ws/src/teleop/config/**teleop_params.yaml**
- build the package ~/ranger_ws/colon build --packages-select teleop
- source the environment $source ~/ranger_ws/install/setup.bash
- run the node $ros2 run teleop teleop_node
- topic  verification $ros2 run topic list

topic /cmd_rov form ADC0 to ADC5  with total 6 analog channels (ros2 topic echo /cmd_rov)  
topic /led_brightess from ADC6 with total 1 analog channels (ros2 topic echo /cmd_led_brightness)  
topic /cmd_arm from ADC7-ADC15 with total 8 analog channels (ros2 topic echo /cmd_arm)  
topic /depth_hold from D22 with total 1 digital channels(ros2 topic echo /depth_hold)  
topic /ai_instruct from D23 to D25 with total 3 digital channels (ros2 topic echo /ai_instruct)

**3. Data visualization with Foxglove Studio**
$ros2 run 
