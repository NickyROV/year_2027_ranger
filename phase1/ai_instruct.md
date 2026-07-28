# /ai_instruct topic data fromat # 

The decision to publish /ai_instruct as a 3-bit integer (0-7) rather than simple boolean (True/False) values is a deliberate design choice driven by both hardware capabilities and software architecture in robotics.
Here is the detailed breakdown of why it is designed this way:</br>

1. Hardware Perspective: Need for Multiple Modes (Not just On/Off)
If the system only needed to turn AI "On" or "Off", a single digital pin (publishing a single Bool) would be sufficient. However, by using 3 digital pins (D23, D24, D25), the hardware can represent 
=8 distinct states (0 to 7).</br>
2. In physical ROV/Robot operation, these 3 pins are typically wired to a rotary switch, a multi-position selector, or a DIP switch. This allows the operator to select specific AI behaviors or mission modes on the fly.
Example of what the 8 states (0-7) could represent:</br>

|Int32 Value|Binary (D25, D24, D23)|Potential AI Mode / Instruction|
|--|--|--|
|0|000|Mode 0: Full Manual Control (AI Disabled)|
|1|001|Mode 1: Auto Depth Hold|
|2|010|Mode 2: Auto Heading / Compass Lock|
|3|011|Mode 3: Vision-based Target Tracking|
|4|100|Mode 4: Autonomous Waypoint Navigation|
|5|101|Mode 5: Pipeline/Structure Following|
|6|110|Mode 6: Emergency Surface / Return to Home|
|7|111|Default/Idle: All switches open (Pull-up state)|

If it were just a Bool, you would lose the ability to select which AI algorithm to run.</br>

3. Software / ROS 2 Perspective: Atomic State & Simplified Logic
Publishing this as a single Int32 (0-7) instead of three separate Bool topics (e.g., /ai_bit1, /ai_bit2, /ai_bit3) offers significant software advantages:</br>
*Atomic State Updates*: Because the 3 bits are packed into a single byte on the Arduino and parsed together in Python, the ROS 2 node publishes the combined state in a single message. This guarantees that the downstream AI node never receives a "mixed" or transitional state (e.g., receiving an update for D23 but not D24 yet).
Simpler Downstream Code: The AI control node only needs to subscribe to one topic (/ai_instruct).</br>
