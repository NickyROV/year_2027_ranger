# year_2027 Ranger Class
## 6DOF ROV with 2 robot arms, each arms with 4 servos, autonomous curising capable

1. Hardward and sensors
* RDK X5 with 10Tops BPU
* 6-axis IMU MPU6050 in I2C interface (address 0x68)
* Water pressure sensors MS5837 in I2C interface (address 0x76)
* Depth sensors DYP-L08 in UART interface 

2. Thruster and robot arm
* 6 x 3-phase T200 Blue Robotic thruster orientiate in Mecanum wheel arrangement 
* 6 x 3-phase ESC with PWM input 
* 2 x 4-servos robot arm
* Thruster 6 x ESCs and Robot Arm 8 x servos are controlled by PCA9685 (address 0x40)

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
