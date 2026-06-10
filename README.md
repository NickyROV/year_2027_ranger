# year_2027 Ranger Class
## 6DOF ROV with 2 robot arms, each arms with 4 servos, autonomous curising capable

1. Hardward and sensors
* RDK X5 with 10Tops BPU
* 6-axis IMU MPU6050 in I2C interface (address 0x68)
* Water pressure sensors MS5837 in I2C interface (address 0x76)
* PCA9685 16 channels PWM in I2C interface (address 0x40 with all jumpers unsoldered) 
* Depth sensors DYP-L08 in UART interface 

2. PWM Thruster, LED Lamp and robot arms
* 6 x 3-phase T200 Blue Robotic thruster orientiate in Mecanum wheel arrangement 
* 6 x 3-phase ESC with PWM input 
* LED Lamp X 1 with PWM input
*  2 x 4-servos robot arm
* Thruster 6 x ESCs, LED lamp X 1 and Robot Arm 8 x servos are controlled by PCA9685 (address 0x40)

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
** Scan I2C devices and confirm that the addresses of PCA9685 (0x40), MPU6050 (0x68), and MS5837 (0x76) are recognizable.
** Write a basic Python script: the PCA9685 outputs a 50Hz square wave to sequentially light up/rotate one servo motor and one T200 thruster ESC.
** Verify DYP-L08 UART data reception (water depth/temperature).
** Output: Able to manually control a single actuator via command line; sensor data can be printed.
