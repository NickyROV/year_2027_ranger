#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200); // Match the baud rate of your Serial Monitor
  while (!Serial);      // Wait for the serial port to connect
  Serial.println("\n--- I2C Scanner Started ---");
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning for I2C devices...");

  nDevices = 0;
  // I2C addresses range from 1 to 126 (0x01 to 0x7E)
  for(address = 1; address < 127; address++ ) {
    
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    // If error is 0, a device acknowledged this address
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      nDevices++;
    } 
    // If error is 4, there was a hardware error on the bus
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  
  if (nDevices == 0) {
    Serial.println("No I2C devices found. Check your wiring!\n");
  } else {
    Serial.println("Scan complete. Waiting 3 seconds...\n");
  }
  
  delay(3000); // Wait 3 seconds before scanning again
}