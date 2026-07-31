# Data Acquisition #

1. IMU ICM-20948 via I2C 0x68 with Mahony filter with outputs both the Quaternions for robotics and Euler Angles for visualization.</br>
*Arduino Reference -> icm20948_mahony.cpp*</br>
Possible issue with Magnetometer : Hard Iron and Soft Iron Distortion, Magnetic Declination.</br>
The Fix : Run a Magnetometer Calibration Sketch, Calibration to Mahony Filter, Add Magnetic Declination to YAW.</br>
As ICM20948 is consumer grade IMU, +/- 5 degree accuracy is expected.</br> 

