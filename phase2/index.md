# Data Acquisition #

1. IMU ICM-20948 via I2C 0x68 (HIGH PRIORITY) with Mahony filter with outputs both the Quaternions for robotics and Euler Angles for visualization.</br>
*Arduino Reference -> icm20948_mahony.cpp*</br>
Possible issue with Magnetometer : Hard Iron and Soft Iron Distortion, Magnetic Declination.</br>
The Fix : Run a Magnetometer Calibration Sketch, Calibration to Mahony Filter, Add Magnetic Declination to YAW.</br>
As ICM20948 is consumer grade IMU, +/- 5 degree accuracy is expected.</br>

2. Underwater Pressure Sensor MS5837 via I2C 0x76 (MEDIUM PRIORITY)</br>

3. 16 Channels PWM Controller PCA9685 via I2C 0x40 (LOW PRIORITY)</br> 

4. MIPI-CSI Camera Connectors on separate I2C Bus (/dev/i2c-4) </br>

5. TJC HMI Status display via UART </br>

6. Ultra sound distance detector DYP-L08 via UART </br>

## Sensor Fusion ##
Combining two sensors with opposite strengths and weaknesses:</br>
IMU (Yaw): High frequency (200Hz+), zero latency, but drifts over time.</br>
Camera (Heading): Low frequency (15-30Hz), slight processing latency, but zero drift (absolute).</br>
The RDK X5 will use an Extended Kalman Filter (EKF) to merge them. The logic looks like this: </br>
1.Between camera frames (e.g., IMU data every 5 milliseconds):</br>
The robot only trusts the IMU. It integrates the Gyro's Z-axis rotation to calculate the change in heading ($\Delta$Yaw).</br>
Current_Heading = Last_Camera_Heading + Integrated_Gyro_Delta</br>
2. When a new camera frame is processed (e.g., camera frame every 50 milliseconds):</br>
The camera calculates the absolute heading relative to the environment. The filter uses this absolute value to instantly correct any drift that accumulated in the IMU.</br>
As ROV are operating underwater, the camera will inevitably fail at some point, it's software must handles these edge cases gracefully:</br>
*Featureless Environments / The Sand Desert Problem)*</br>
If the robot drifts over smooth, featureless sand, the camera has no distinct features to track. Visual heading will drop to zero confidence.</br>
The Fix: Your code must monitor the "confidence score" of the visual tracking. If confidence drops below a threshold, the system must freeze the camera heading and switch to Pure IMU Dead Reckoning until features reappear.</br>
*Silt and Murky water*</br>
If the thrusters kick up sand, or a fish swims by and blocks the lens, the camera goes blind.</br>
The Fix: Same as above. Fall back to the IMU. Because the robot is only "guarding" a small area, the IMU's yaw drift over a 5-second silt cloud will only be a few degrees—perfectly acceptable for short-term station keeping.</br>
*The Gimbal Lock of the Camera*</br>
If the camera is pointing straight down at the seabed, and the robot spins perfectly on its Z-axis (Yaw), the camera sees the exact same image rotating. This is easy for Visual SLAM to track. However, if the camera is pointing forward and the robot pitches up/down, you lose tracking.</br>
The Fix: For a stationary guarding robot, point the camera straight down at the seabed. This provides a perfect 2D plane for Visual Odometry to track X, Y, and Yaw simultaneously without perspective distortion.</br>
