# year_2027 Ranger Class
## 6DOF ROV with 2 robot arms, each arms with 4 servos, autonomous curising capable

1. Hardward and sensors
* RDK X5 with 10Tops BPU (3V3 logic level)
* I2C total 4K7 pull-up both SCL & SDA; maximum distance in between 15cm
* 9-axis IMU ICM-20948 V2 in 3V3 I2C interface (address 0x69)
* Water pressure sensors MS5837 in 3V3 I2C interface (address 0x76)
* PCA9685 16 channels PWM 5V in I2C interface (address 0x40) with 470uF across VCC and GND to reduce Electromagnetic Interference (EMI)
* Level shifter TXS0108E for PCA9685 shifting
* Depth sensors DYP-L08 in UART interface 

2. PWM Thruster, LED Lamp and robot arms
* 8 x 3-phase T200 Blue Robotic thruster orientiate in Mecanum wheel arrangement 
* 8 x 3-phase ESC with PWM input
* Moore-Penrose Pseudoinverse will be adopted as 6X8 Matrix (6DOF commands into 8 thrusters) is overactuated to reduce cross-coupling and enhance energy efficiency. 
* 2 x 4-servos robot arm
* Thruster 8 x ESCs and Robot Arm 8 x servos are controlled by PCA9685 (address 0x40)
* LED Lamp using 20KHz PWM signal output from RDK GPIO pin

3. Computer vision
* To recognize various sea creature and coral reef
* Object follows
* Dimension measuring with IMU integrated with time, NO Doppler Velocity Logs
* Bathemetry with depth sensor, NO MultiBeam Echosounder

4. Topology
* Top-side computer for live feed video with flight data embedded visual display
* Radio Controller send command throu 16 channel SBUS
* RDK X5 station underwater compactment, close to sensor
* Top-side computer and underwater compartment are connected by tether

5. Implementation Roadmap
* Phase 1 (Hardware Verification): Connect RDK X5 and PCA9685, write basic scripts, activate the servos and T200 thrusters one by one, and verify I2C communication stability.
** Burn the RDK X5 system image and confirm network and SSH connections.
** Scan I2C devices and confirm that the addresses of PCA9685 (0x40), ICM-20948 (0x69), and MS5837 (0x76) are recognizable.
** Write a basic Python script: the PCA9685 outputs a 50Hz square wave to sequentially light up/rotate one servo motor and one T200 thruster ESC.
** Verify DYP-L08 UART data reception (water depth/temperature).
** Output: Able to manually control a single actuator via command line; sensor data can be printed.

* Phase 2 (Watertightness and Power): Complete the pressure hull design, test the T200's forward and reverse thrust response in water, and verify the ICM-20948 attitude readout.
** Complete the structural design and assembly of the pressure tank (watertight joints, cable penetration).
** Establish a ROS 2 workspace (TROS.b environment) and write the following nodes:
** imu_node: Publish the ICM-20948's roll/pitch/yaw (including magnetometer yaw)
** depth_node: Publish the MS5837 depth
** Implement a single-degree-of-freedom PID controller (e.g., constant depth control), maintaining the target depth ±5cm by adjusting the vertical thruster PWM.
** Output: The ROV can achieve "constant depth hovering" in still water, no longer relying on continuous manual correction at the water surface.

* Phase 3 (Semi-Autonomous Cruise): Deploy TROS.b (TogetherROS) on RDK X5, and write nodes to implement "depth-hold mode" and "self-stabilizing mode".
** Establish thrust distribution matrix (Mecanum-type 8-thruster layout: 1-4 for Surge/Sway/Yaw, 5-8 for Heave/Pitch/Roll)
** Subscribe to /cmd_vel and convert to 8-channel PWM output (using Moore-Penrose Pseudoinverse)
** Connect to SBUS remote controller (16 channels), map joystick values ​​to /cmd_vel, while retaining ROS command-line control
** Add auto_heading node: use the integrated magnetometer from ICM-20948 to correct yaw drift
** Output: ROV can be controlled forward/backward/left/right/heave/turn by remote controller; automatically returns to stability when joystick is released.

* Phase 4 (AI Vision): Collect underwater image datasets, deploy a lightweight YOLO model on RDK X5, and write target following logic.
**Collect and label underwater datasets (fish, sea cucumbers, corals, etc.).
** Deploy YOLOv8 (or a lightweight version) using the DNN node in TROS.b to obtain object detection boxes.
** Write a visual_servo node to convert the object offset (px error) within the image box into a /cmd_vel horizontal correction command.
** Combined with IMU depth information, implement 3D following (target remains centered in the field of view + constant depth following).
**Output: The ROV can autonomously follow specified marine life within its field of view, maintaining a fixed distance and depth.

* Phase 5 (Integration): Write surface-end Qt/Python programs to integrate video streams and the instrument panel.
**The surface computer runs Qt/PyQt programs:
** Video Streaming: Receives underwater camera footage via GStreamer (H.264) and overlays OSD (depth, attitude, bounding box).
** Remote Controller Pass-through: Sends /cmd_vel from the surface to the underwater RDK X5.
** DVL-less Visual Ranging: Estimates short-term displacement using IMU + visual feature point method (viso2_ros).
** Simple Bathymetry: Combines displacement estimation with MS5837 depth data to generate a raster bathymetry map (saved as a CSV/PNG heatmap).
** Output: The surface screen displays "flight data + AI recognition + real-time video," and the ROV can collect underwater topographic data along the planned route.

