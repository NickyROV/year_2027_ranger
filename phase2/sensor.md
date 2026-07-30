# Sensors and actuator 
0. I2C scanning</br>
Always perform I2C scanning to ensure I2C address (Don't take data sheet for granted as breakout break may alter address)</br>
Arduino UNO is always the best device to perform I2C scanning, check I2C_Scan_ArduinoUNO.cpp</br>

1. I2C 0x68 ICM20948 9-Axis IMU</br>
Resources : https://github.com/sparkfun/SparkFun_ICM-20948_ArduinoLibrary</br>
The SparkFun breakout board features built-in level shifters, so it can safely interface with both 3.3V and 5V microcontrollers.</br>
For standard I2C communication, you only strictly need 4 pins: VCC, GND, SCL, and SDA </br>
The remaining 6 pins can safely leave floating, those are designed with built-in resistors to ensure these pins default state when floating.</br>
**Note** - The ICM-20948 chip actually has two separate I2C interfaces built into it with *Primary SDA/SCL* and *Auxiliary EDA/ECL as secondary I2C Master to talk to external sensors*</br>
*ADO* - Address Select to change from 0x68 to 0x69 </br>
*NCS* - SPI Chip Select in SPI communication</br>
*INT* - Hardware Interrupt Output to notify microcontroller that new data is ready</br>
*FSYNC* - Frame Synchronization pin to presisely timestamp IMU data to match external event</br>


2. I2C MS5837 Underwater Pressure</br>

3. UART DYPL08 Ultra-Sonic distance sensor</br>

4. I2C PCA9685 16-channel PWM Controller</br>

5. PWM Underwater LED</br>
