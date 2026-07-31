#include "ICM_20948.h"
#include <Wire.h>

#define AD0_VAL 0
ICM_20948_I2C myICM;

float magX_min = 1000, magX_max = -1000;
float magY_min = 1000, magY_max = -1000;
float magZ_min = 1000, magZ_max = -1000;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Wire.begin();
  Wire.setClock(400000);
  
  while (myICM.begin(Wire, AD0_VAL) != ICM_20948_Stat_Ok) {
    Serial.println("Sensor not found, retrying...");
    delay(500);
  }
  
  Serial.println("=== MAGNETOMETER CALIBRATION ===");
  Serial.println("Rotate the sensor slowly in ALL directions (figure-8 motions).");
  Serial.println("Make sure to tilt it every which way for 30 seconds.");
  Serial.println("Press any key + Enter to start...");
  while (!Serial.available());
  while (Serial.available()) Serial.read(); // clear buffer
  
  unsigned long startTime = millis();
  while (millis() - startTime < 30000) { // 30 seconds
    if (myICM.dataReady()) {
      myICM.getAGMT();
      float mx = myICM.magX();
      float my = myICM.magY();
      float mz = myICM.magZ();
      
      if (mx < magX_min) magX_min = mx;
      if (mx > magX_max) magX_max = mx;
      if (my < magY_min) magY_min = my;
      if (my > magY_max) magY_max = my;
      if (mz < magZ_min) magZ_min = mz;
      if (mz > magZ_max) magZ_max = mz;
    }
  }
  
  // Calculate hard-iron offsets (center of the sphere)
  float magX_offset = (magX_max + magX_min) / 2.0f;
  float magY_offset = (magY_max + magY_min) / 2.0f;
  float magZ_offset = (magZ_max + magZ_min) / 2.0f;
  
  // Calculate scale factors (average radius)
  float magX_scale = (magX_max - magX_min) / 2.0f;
  float magY_scale = (magY_max - magY_min) / 2.0f;
  float magZ_scale = (magZ_max - magZ_min) / 2.0f;
  float avg_radius = (magX_scale + magY_scale + magZ_scale) / 3.0f;
  
  // Normalize scale factors
  magX_scale = avg_radius / magX_scale;
  magY_scale = avg_radius / magY_scale;
  magZ_scale = avg_radius / magZ_scale;
  
  Serial.println("\n=== CALIBRATION COMPLETE ===");
  Serial.println("Copy these values into your Mahony filter code:\n");
  Serial.print("float magX_offset = "); Serial.print(magX_offset, 2); Serial.println(";");
  Serial.print("float magY_offset = "); Serial.print(magY_offset, 2); Serial.println(";");
  Serial.print("float magZ_offset = "); Serial.print(magZ_offset, 2); Serial.println(";");
  Serial.print("float magX_scale  = "); Serial.print(magX_scale, 3); Serial.println(";");
  Serial.print("float magY_scale  = "); Serial.print(magY_scale, 3); Serial.println(";");
  Serial.print("float magZ_scale  = "); Serial.print(magZ_scale, 3); Serial.println(";");
  
  Serial.println("\n--- Raw Ranges Detected ---");
  Serial.print("X: "); Serial.print(magX_min); Serial.print(" to "); Serial.println(magX_max);
  Serial.print("Y: "); Serial.print(magY_min); Serial.print(" to "); Serial.println(magY_max);
  Serial.print("Z: "); Serial.print(magZ_min); Serial.print(" to "); Serial.println(magZ_max);
}

void loop() { }