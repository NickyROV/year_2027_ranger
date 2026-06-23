# Magnetic field induced by Blue Robotic T200 3-phase thruster
General test on how 9-Axis IMU affected by T200 

All reading with Earth Magnetic Field Strength (~ 50uT) corrected

|Distance|6V Static Motor|6V Motor in action|Static T200| 12V/1.9ms PWM feed to T200|
|---|---|---|---|---|
|20cm|0|0|0|0|
|10cm|3uT|3uT|0|0|
|5cm|4uT|4uT|0|0|
|2cm|5uT|5uT|0|0|
|Proximity|>50uT|>50uT|0|0|

6V permanent magnet motor serve as reference control, it seems T200 is magnet-free 3-phase thruster

Conclusion : it's safe to assume 9-axis IMU magnetic heading will not be affected by thrusters, however, it's still good practice to keep a distance from them.
