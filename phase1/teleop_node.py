#!/usr/bin/env python3
"""
Teleop Node - Phase 1
Reads serial data from Arduino Mega and publishes ROS2 topics

Topics published:
  /cmd_rov      - ADC0-ADC5 (6 channels) -> Int32MultiArray
  /led_brightness - ADC6 (1 channel)     -> Int32
  /cmd_arm      - ADC7-ADC15 (8 channels)-> Int32MultiArray  
  /depth_hold   - D22 (1 bit)            -> Bool
  /ai_instruct  - D23-D25 (3 bits)       -> Int32 (0-7)
"""

import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32MultiArray, Int32, Bool
import serial
import struct
import time
import threading

class TeleopNode(Node):
    def __init__(self):
        super().__init__('teleop_node')
        
        # Parameters
        self.declare_parameter('serial_port', '/dev/ttyUSB0')
        self.declare_parameter('baud_rate', 115200)
        self.declare_parameter('update_rate_hz', 50)
        
        port = self.get_parameter('serial_port').value
        baud = self.get_parameter('baud_rate').value
        self.update_rate = self.get_parameter('update_rate_hz').value
        
        # Initialize serial
        try:
            self.ser = serial.Serial(port, baud, timeout=0.1)
            self.get_logger().info(f"Connected to {port} at {baud} baud")
        except Exception as e:
            self.get_logger().error(f"Failed to open serial port: {e}")
            raise
        
        # Publishers
        self.cmd_rov_pub = self.create_publisher(Int32MultiArray, '/cmd_rov', 10)
        self.led_brightness_pub = self.create_publisher(Int32, '/led_brightness', 10)
        self.cmd_arm_pub = self.create_publisher(Int32MultiArray, '/cmd_arm', 10)
        self.depth_hold_pub = self.create_publisher(Bool, '/depth_hold', 10)
        self.ai_instruct_pub = self.create_publisher(Int32, '/ai_instruct', 10)
        
        # Frame buffer
        self.buffer = bytearray()
        self.frame_size = 36  # 2 header + 32 analog + 1 digital + 1 footer
        
        # Start reading thread
        self.running = True
        self.read_thread = threading.Thread(target=self._read_loop)
        self.read_thread.daemon = True
        self.read_thread.start()
        
        self.get_logger().info("Teleop node initialized")
    
    def _read_loop(self):
        """Background thread for reading serial data"""
        while self.running and rclpy.ok():
            try:
                if self.ser.in_waiting > 0:
                    data = self.ser.read(self.ser.in_waiting)
                    self.buffer.extend(data)
                    
                    # Process complete frames
                    while len(self.buffer) >= self.frame_size:
                        # Find frame start
                        start_idx = self.buffer.find(b'\xAA\xBB')
                        if start_idx == -1:
                            # No valid header found, clear buffer
                            self.buffer.clear()
                            break
                        
                        if start_idx > 0:
                            # Remove garbage before header
                            self.buffer = self.buffer[start_idx:]
                            continue
                        
                        # Check if we have a complete frame
                        if len(self.buffer) >= self.frame_size:
                            # Verify footer
                            if self.buffer[self.frame_size - 1] == 0xCC:
                                # Parse and publish
                                self._parse_and_publish(self.buffer[:self.frame_size])
                                # Remove processed frame
                                self.buffer = self.buffer[self.frame_size:]
                            else:
                                # Invalid frame, remove header and continue
                                self.buffer = self.buffer[2:]
                        else:
                            break
            except Exception as e:
                self.get_logger().warn(f"Serial read error: {e}")
            
            time.sleep(0.001)  # Small sleep to prevent CPU hogging
    
    def _parse_and_publish(self, frame):
        """Parse binary frame and publish ROS2 messages"""
        try:
            # Frame format: AA BB [A0H A0L ... A15H A15L] [DIGITAL] CC
            # Skip header (2 bytes) and footer (1 byte)
            analog_data = frame[2:34]  # 32 bytes
            digital_byte = frame[34]    # 1 byte
            
            # Parse analog values (16 channels, 10-bit each)
            analog_vals = []
            for i in range(16):
                high = analog_data[i*2]
                low = analog_data[i*2 + 1]
                val = (high << 8) | low
                analog_vals.append(val)
            
            # Map to topics
            # ADC0-ADC5 -> /cmd_rov (6 channels)
            cmd_rov_msg = Int32MultiArray()
            cmd_rov_msg.data = analog_vals[0:6]
            self.cmd_rov_pub.publish(cmd_rov_msg)
            
            # ADC6 -> /led_brightness (1 channel)
            led_msg = Int32()
            led_msg.data = analog_vals[6]
            self.led_brightness_pub.publish(led_msg)
            
            # ADC7-ADC15 -> /cmd_arm (8 channels)
            cmd_arm_msg = Int32MultiArray()
            cmd_arm_msg.data = analog_vals[7:15]
            self.cmd_arm_pub.publish(cmd_arm_msg)
            
            # Digital bits
            # D22 -> /depth_hold (bit 0)
            depth_hold_msg = Bool()
            depth_hold_msg.data = bool(digital_byte & 0x01)
            self.depth_hold_pub.publish(depth_hold_msg)
            
            # D23-D25 -> /ai_instruct (bits 1-3)
            ai_value = (digital_byte >> 1) & 0x07  # bits 1,2,3 -> 0-7
            ai_msg = Int32()
            ai_msg.data = ai_value
            self.ai_instruct_pub.publish(ai_msg)
            
            # Log at reduced rate
            if not hasattr(self, '_log_counter'):
                self._log_counter = 0
            self._log_counter += 1
            if self._log_counter % 50 == 0:
                self.get_logger().debug(
                    f"Published: /cmd_rov={analog_vals[0:6]}, "
                    f"/led={analog_vals[6]}, "
                    f"/depth_hold={bool(digital_byte & 0x01)}, "
                    f"/ai_instruct={ai_value}"
                )
                
        except Exception as e:
            self.get_logger().error(f"Parse error: {e}")
    
    def destroy_node(self):
        self.running = False
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()
        super().destroy_node()

def main(args=None):
    rclpy.init(args=args)
    node = TeleopNode()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info("Shutting down...")
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()