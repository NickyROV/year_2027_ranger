# Sensors and actuator #
0. I2C scanning</br>
Always perform I2C scanning to ensure I2C address (Don't take data sheet for granted as breakout break may alter address)</br>
Arduino UNO is always the best device to perform I2C scanning, check I2C_Scan_ArduinoUNO.cpp</br>

2. I2C ICM20948 9-Axis IMU</br>
Resources : https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary</br>
The SparkFun breakout board features built-in level shifters, so it can safely interface with both 3.3V and 5V microcontrollers.</br>
For standard I2C communication, you only strictly need 4 pins: VCC, GND, SCL, and SDA </br>
The remaining 6 pins can safely leave floating, those are designed with built-in resistors to ensure these pins default state when floating.</br>


4. I2C MS5837 Underwater Pressure
5. UART DYPL08 Ultra-Sonic distance sensor
6. I2C PCA9685 16-channel PWM Controller
7. PWM Underwater LED
