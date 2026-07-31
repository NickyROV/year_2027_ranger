# Data Acquisition #

1. IMU ICM-20948 via I2C 0x68 with Mahony filter with outputs both the Quaternions for robotics and Euler Angles for visualization.</br>
*Arduino Reference -> icm20948_mahony.cpp*</br>
Possible issue with Magnetometer : Hard Iron and Soft Iron Distortion, Magnetic Declination.</br>
The Fix : Run a Magnetometer Calibration Sketch, Calibration to Mahony Filter, Add Magnetic Declination to YAW.</br>
As ICM20948 is consumer grade IMU, +/- 5 degree accuracy is expected.</br> 

## Sensor Fusion ##
Combining two sensors with opposite strengths and weaknesses:</br>
IMU (Yaw): High frequency (200Hz+), zero latency, but drifts over time.</br>
Camera (Heading): Low frequency (15-30Hz), slight processing latency, but zero drift (absolute).</br>
The RDK X5 will use an Extended Kalman Filter (EKF) to merge them. The logic looks like this: </br>
1.Between camera frames (e.g., every 5 milliseconds):</br>
The robot only trusts the IMU. It integrates the Gyro's Z-axis rotation to calculate the change in heading ($\Delta$Yaw).</br>
Current_Heading = Last_Camera_Heading + Integrated_Gyro_Delta</br>
2. When a new camera frame is processed (e.g., every 50 milliseconds):</br>
The camera calculates the absolute heading relative to the environment. The filter uses this absolute value to instantly correct any drift that accumulated in the IMU.</br>
