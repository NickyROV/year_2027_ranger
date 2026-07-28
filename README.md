# year_2027 Ranger Class
## 6DOF ROV with 2 X 4 servos robot arms, autonomous curising capable

1. **Hardward and sensors**
* Top-side GroundControl ranger_ws SoC : Raspberry pi 5 + Arduino mega 2560</br>
* Wet-side wtc_ws CPU+GPU+BPU : RDK X5 with 10Tops BPU (3V3 logic level)</br>
* I2C total 4K7 pull-up both SCL & SDA; maximum distance in between 15cm</br>
* 9-axis IMU ICM-20948 V2 in 3V3 I2C interface (address 0x69)</br>
* Water pressure sensors MS5837 in 3V3 I2C interface (address 0x76)</br>
* PCA9685 16 channels PWM 5V in I2C interface (address 0x40) with 470uF across VCC and GND to reduce Electromagnetic Interference (EMI)</br>
* Level shifter TXS0108E for PCA9685 shifting</br>
* Depth sensors DYP-L08 in UART interface</br>
* MIPI DSI 4.3-inch Display</br>
* MIPI CSI Camera (77-view angle) with Sony MX219 chip with 8.08-megaPixel (3280x2464 active Pixel) -> mipi_host0 (i2c6)</br>

2. **PWM Thruster, LED Lamp and robot arms**
* 8 x 3-phase T200 Blue Robotic thruster orientiate in Mecanum wheel arrangement</br>
* 8 x 3-phase ESC with PWM input</br>
* Moore-Penrose Pseudoinverse will be adopted as 6X8 Matrix (6DOF commands into 8 thrusters) is overactuated to reduce cross-coupling and enhance energy efficiency.</br> 
* 2 x 4-servos robot arm</br>
* Thruster 8 x ESCs and Robot Arm 8 x servos are controlled by PCA9685 (address 0x40)</br>
* LED Lamp using 20KHz PWM signal output from RDK GPIO pin</br>

3. **Computer vision**
* To recognize various sea creature and coral reef</br>
* Object follows</br>
* Dimension measuring with IMU integrated with time, NO Doppler Velocity Logs</br>
* Bathemetry with depth sensor, NO MultiBeam Echosounder</br>

4. **Topology**
* Top-side computer for live feed video with flight data embedded visual display</br>
* Radio Controller send command throu 16 channel SBUS</br>
* RDK X5 station underwater compactment, close to sensor</br>
* Top-side computer and underwater compartment are connected by tether</br>

5. **Implementation Roadmap**
* Phase 1 (Ground Control framework)</br>
** Analog ADC : 6 channel Teleop_ROV, 8 channels Robot Rrm and 1 channel of LED Brightness -> Arduino Mega</br>
** Digital : Depth Hold, AI Instruction -> Arduino Mega</br>
** Arduino Mega -> Pi 5 via Serial Port</br>
** Pi 5 publish corresponding Topics</br>
   
* Phase 2 (Hardware Verification): Connect RDK X5 and PCA9685, write basic scripts, activate the servos and T200 thrusters one by one, and verify I2C communication stability.</br>
** Burn the RDK X5 system image and confirm network and SSH connections.</br>
** Address confirmation of I2C devices with proper level shifter; PCA9685 (0x40), ICM-20948 (0x69), and MS5837 (0x76).</br>
** Python executable -> prepare 50Hz PWM output to PCA9685 to control all 8 T200 thruster Electronic Speed Controllers.</br>
** Verify DYP-L08 UART data reception (water depth/temperature).</br>
** Output: Able to manually control a single actuator via command line; sensor data can be printed.</br>

* Phase 3 (Watertightness and Power): Water tight compactment design, test 8 X T200's thruster performance, verify the ICM-20948 attitude readout.</br>
** Structural design and assembly of the pressure tank (watertight joints, cable penetration).</br>
** Establish a ROS 2 workspace (TROS.b environment) and write the following nodes:</br>
** /sense_icm20948 : publish /icm20948 topic, 9-axis IMU</br>
** /sense_ms5837 : publish /ms5837 topic, depth by converting the pressure</br>
** /sense_dypl08 : publish /dypl08 topic, Ultra-sound distance</br>
** Implement a single-degree-of-freedom PID controller (e.g., constant depth control), maintaining the target depth ±5cm by adjusting the vertical thruster PWM.</br>
** Output: The ROV can achieve "constant depth hovering" in stream water, NO depth manual-correct is needed.</br>
  
* Phase 4 (Semi-Autonomous Cruise): Deploy TROS.b (TogetherROS) on RDK X5, and write nodes to implement "depth-hold mode" and "self-stabilizing mode".</br>
** Establish thrust distribution matrix (Mecanum-type 8-thruster layout: 1-4 for Surge/Sway/Yaw, 5-8 for Heave/Pitch/Roll)</br>
** Decompose 8 DoFs matrix into 2 independent 4 DoFs matrixes, Pseudoinverse underdetermined martix [3x4].</br>
** [τ<sub>surge</sub>,τ<sub>sway</sub>,τ<sub>yaw</sub>] Pesudoinverse [A <sub>horizontial</sub> 3X4] into [μ<sub>1</sub>,μ<sub>2</sub>,μ<sub>3</sub>,μ<sub>4</sub>] actuators output.</br>
** [τ<sub>heavr</sub>,τ<sub>pitch</sub>,τ<sub>roll</sub>] Pesudoinverse [A <sub>vertical</sub> 3X4] into [μ<sub>5</sub>,μ<sub>6</sub>,μ<sub>7</sub>,μ<sub>8</sub>] actuators output.</br>
** Subscribe to /cmd_vel and convert to 8-channel PWM output (using Moore-Penrose Pseudoinverse)</br>
** Connect to SBUS remote controller (16 channels), map joystick values ​​to /cmd_vel, while retaining ROS command-line control</br>
** Add auto_heading node: use the integrated magnetometer from ICM-20948 to correct yaw drift</br>
** Output: ROV can be controlled forward/backward/left/right/heave/turn by remote controller; automatically returns to stability when joystick is released.</br>

* Phase 5 (AI Vision): Collect underwater image datasets, deploy a lightweight YOLO model on RDK X5, and write target following logic.</br>
** Collect and label underwater datasets (fish, sea cucumbers, corals, etc.).</br>
** Deploy YOLOv8 (or a lightweight version) using the DNN node in TROS.b to obtain object detection boxes.</br>
** Write a visual_servo node to convert the object offset (px error) within the image box into a /cmd_vel horizontal correction command.</br>
** Combined with IMU depth information, implement 3D following (target remains centered in the field of view + constant depth following).</br>
** Output: The ROV can autonomously follow specified marine life within its field of view, maintaining a fixed distance and depth.</br>

* Phase 6 (Integration): Write surface-end Qt/Python programs to integrate video streams and the instrument panel.
**T he surface computer runs Qt/PyQt programs:</br>
** Video Streaming: Receives underwater camera footage via GStreamer (H.264) and overlays OSD (depth, attitude, bounding box).</br>
** Remote Controller Pass-through: Sends /cmd_vel from the surface to the underwater RDK X5.</br>
** DVL-less Visual Ranging: Estimates short-term displacement using IMU + visual feature point method (viso2_ros).</br>
** Simple Bathymetry: Combines displacement estimation with MS5837 depth data to generate a raster bathymetry map (saved as a CSV/PNG heatmap).</br>
** Output: The surface screen displays "flight data + AI recognition + real-time video," and the ROV can collect underwater topographic data along the planned route.

