# Ground Control Station Architect
## Hardware : Ubuntu Jammy with ROS2 Humble + Arduino Mega 2560
 
| Hardware(Arduino) │───▶|  node /teleop  │───▶│ topic /cmd_rov │───▶│ wtc Controller nodes │

**1. Arduino Mega 2560**

Deploy Arduino Mega to read ADC and Digital teleoperating command, wrap up all information in serial format and pass to GroundControl through /dev/ttyUSB0;
- 16 ADC from ADC0 to ADC15
- 8 Digital from D22 to D29
- Arduino code reference *teleop.ino*
  
**2. GroundControl unit - Ubuntu Jammy with ROS2 Humble**

ranger_ws with /teleop node to read UART and publish topic
- Create ranger_ws ROS2 workspace ~/ranger_ws
- Create teleop ROS2 Package ~$ros2 pkg create teleop 
- Update ~/ranger_ws/src/teleop/*package.xml*
- Update ~/ranger_ws/src/teleop/*setup.py*
- Create ROS2 python teleop_node  ~/ranger_ws/src/teleop/teleop/*teleop_node.py*
- Create ROS2 parameters file ~/ranger_ws/src/teleop/config/*teleop_params.yaml*
- build the package ~/ranger_ws/colon build --packages-select teleop
- source the environment $source ~/ranger_ws/install/setup.bash
- run the node $ros2 run teleop teleop_node
- topic  verification $ros2 run topic list

topic /cmd_rov form ADC0 to ADC5  with total 6 analog channels (ros2 topic echo /cmd_rov)  
topic /led_brightess from ADC6 with total 1 analog channels (ros2 topic echo /cmd_led_brightness)  
topic /cmd_arm from ADC7-ADC15 with total 8 analog channels (ros2 topic echo /cmd_arm)  
topic /depth_hold from D22 with total 1 digital channels(ros2 topic echo /depth_hold)  
topic /ai_instruct from D23 to D25 with total 3 digital channels (ros2 topic echo /ai_instruct)

**2. Optional upgrade regarding dynamic /dev/ttyUSB0 assignment**
As ubuntu assign USB device dynamically, chances are Arduino Mega got different port assignment than /dev/ttyUSB0, as required by teleop_node.py, user can't force a specific ttyUSB number directly using udev rules on modern Ubuntu, because the kernel's naming system assigns these numbers dynamically . However, the standard solution is to create a stable, persistent symlink (like /dev/arduino) that always points to your Arduino, no matter what ttyUSB number it gets.
- Step 1 : Identify Arduino's Unique Hardware Information {idVendor} & {idProduct} with lsusb
- Step 2 : Create udev Rule in /etc/udev/rules.d/99-usb-serial.rules by adding "
- Step 3 : Rule application by sudo udevadm control --reload-rules & sudo udevadm trigger
- Step 4 : New Symlink vertification by ls -l /dev/arduino
- Step 5 : change *teleop_params.yaml* serial_port:"/dev/ttyUSB0" to serial_port:"/dev/arduino"
- Step 6 : call parameter *teleop_params.yaml* explicitly by ros2 run teleop teleop_node --ros-args --params-file ~/ranger_ws/src/teleop/config/teleop_params.yaml
  
**3. Data visualization with Foxglove Studio**
- Installation $sudo apt install foxglove-studio
- apply foxglove-bridge $ros2 launch foxglove_bridge foxglove_bridge_launch.xml
- Open Foxglove WebSocket connection ws://localhost:8765 for foxglove_bridge
- 
