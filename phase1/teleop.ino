/*
 * Arduino Mega Teleop Firmware - Phase 1
 * Reads 16 Analog channels (ADC0-ADC15) and 8 Digital channels (D22-D29).
 *   - ADC0-ADC15 (16 analog channels)
 *   - D22-D29 (8 digital channels with pull-ups)
 * Sends data as binary frame over Serial at 115200 baud
 * 
 * Pin Mapping:
 *   ADC0-ADC5  → /cmd_rov (ROV control)
 *   ADC6       → /led_brightness
 *   ADC7-ADC14 → /cmd_arm (Arm control)
 *   AD15       → /(reserved for future use)
 *   D22        → /depth_hold
 *   D23-D25    → /ai_instruct
 *   D26-D29    → (Reserved for future use)
 * 
 * Upload to: Arduino Mega 2560
 * Serial: USB (COM port on Windows, /dev/ttyUSB* on Linux)
 * Data packages 36-byte binary frame:
 * Header: 0xAA 0xBB (2 bytes)
 * Analog Data: 16 values × 2 bytes each = 32 bytes
 * Digital Data: 1 byte (8 bits packed)
 * Footer: 0xCC (1 byte)
 * Sends this frame over Serial at 115200 baud every 20ms (50Hz)
 */

// ==================== CONFIGURATION ====================
#define NUM_ANALOG  16
#define NUM_DIGITAL 8
#define SERIAL_BAUD 115200
#define UPDATE_RATE_MS 20  // 50Hz update rate

// Digital pins D22-D29
const int digitalPins[NUM_DIGITAL] = {22, 23, 24, 25, 26, 27, 28, 29};

// Frame structure (36 bytes total)
// [Header 2 bytes] [Analog 32 bytes] [Digital 1 byte] [Footer 1 byte]
// Header: 0xAA 0xBB
// Analog: 16 channels × 2 bytes (high byte + low byte)
// Digital: 8 bits packed into 1 byte
// Footer: 0xCC

// ==================== SETUP ====================
void setup() {
  // Initialize Serial communication
  Serial.begin(SERIAL_BAUD);
  
  // Wait for Serial to stabilize (important for USB)
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Configure digital pins as inputs with internal pull-ups
  // This ensures stable readings when switches are not connected
  for (int i = 0; i < NUM_DIGITAL; i++) {
    pinMode(digitalPins[i], INPUT_PULLUP);
  }
  
  // Print startup message to confirm firmware is running
  Serial.println("Arduino Mega Teleop Firmware v1.0");
  Serial.println("Ready to send data...");
  
  // Small delay to let everything stabilize
  delay(100);
}

// ==================== MAIN LOOP ====================
void loop() {
  // Create frame buffer
  uint8_t frame[36];
  int idx = 0;
  
  // === 1. Write Frame Header ===
  frame[idx++] = 0xAA;  // Start byte 1
  frame[idx++] = 0xBB;  // Start byte 2
  
  // === 2. Read 16 Analog Channels ===
  // ADC0-ADC5   → /cmd_rov (6 channels)
  // ADC6        → /led_brightness (1 channel)
  // ADC7-ADC14  → /cmd_arm (8 channels)
  // ADC15      → Not connected (reserved)
  
  for (int i = 0; i < NUM_ANALOG; i++) {
    // Read 10-bit analog value (0-1023)
    int val = analogRead(i);
    
    // Store as high byte and low byte
    frame[idx++] = (val >> 8) & 0xFF;   // High byte
    frame[idx++] = val & 0xFF;          // Low byte
  }
  
  // === 3. Read 8 Digital Channels ===
  // D22  → /depth_hold (bit 0)
  // D23  → /ai_instruct (bit 1)
  // D24  → /ai_instruct (bit 2)
  // D25  → /ai_instruct (bit 3)
  // D26  → Reserved (bit 4)
  // D27  → Reserved (bit 5)
  // D28  → Reserved (bit 6)
  // D29  → Reserved (bit 7)
  
  uint8_t digitalByte = 0;
  for (int i = 0; i < NUM_DIGITAL; i++) {
    // Read digital state (HIGH = 1, LOW = 0)
    int state = digitalRead(digitalPins[i]);
    
    // Set the corresponding bit
    // NOTE: INPUT_PULLUP means switch to GND gives LOW (0)
    //       Open circuit gives HIGH (1)
    if (state == HIGH) {
      digitalByte |= (1 << i);  // Set bit i to 1
    }
    // else bit remains 0
  }
  frame[idx++] = digitalByte;
  
  // === 4. Write Frame Footer ===
  frame[idx++] = 0xCC;  // End byte
  
  // === 5. Send Frame ===
  Serial.write(frame, sizeof(frame));
  
  // === 6. Wait for next cycle (50Hz) ===
  delay(UPDATE_RATE_MS);
}
