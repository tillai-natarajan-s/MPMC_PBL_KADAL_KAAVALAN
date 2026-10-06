KADAL KAAVALAN
IMBL Boundary Detection and Alert System for Indian Fishermen.

Overview
Kadal Kaavalan is an embedded safety system designed to provide fishermen with an early warning when their vessel approaches the International Maritime Boundary Line (IMBL).
The system operates independently of cellular networks and internet connectivity. It combines GPS-based geofencing, ESP32 processing, 433 MHz LoRa communication, local visual/audible alerts, and a shore-side monitoring dashboard.
The prototype is designed to provide a low-cost, standalone last-mile safety mechanism for fishing vessels operating near maritime boundaries.

Problem Statement
Fishing communities operating near the India–Sri Lanka maritime boundary face the risk of unintentionally approaching or crossing the IMBL. Existing solutions may depend on smartphones, internet connectivity, or satellite-based services, which may not always be practical for small-scale fishing vessels.
Kadal Kaavalan addresses this gap by providing a dedicated onboard device that can detect proximity to the IMBL locally and alert the fisherman without requiring cellular or internet connectivity.

Key Features
- Offline GPS-based IMBL geofence detection
- Perpendicular-distance calculation from vessel position to the IMBL polyline
- Three-level proximity indication:
  - Green: Safe
  - Yellow: Warning — within 5 km
  - Red: Alert — within 2 km
- Audible buzzer alert during critical boundary proximity
- Dedicated SOS push button
- 433 MHz LoRa communication using SX1278 Ra-02
- Communication between multiple boat units and a shore gateway
- Shore-side real-time monitoring dashboard
- JSON-based communication between the shore ESP32 and PC
- CSV event logging
- Operation without cellular or internet infrastructure

System Architecture

The system consists of two major sections:
Boat Unit
Each boat unit is based on an ESP32-WROOM-32 microcontroller and includes:
- GPS interface
- SX1278 Ra-02 LoRa transceiver
- 433 MHz antenna
- Green, yellow, and red LEDs
- Active buzzer
- SOS push button
- Power supply
The ESP32 processes the vessel's position, evaluates its distance from the IMBL, determines the current safety state, activates local alerts, and transmits relevant events through LoRa.

Shore Unit
The shore station consists of:
- ESP32-WROOM-32
- SX1278 Ra-02 LoRa transceiver
- 433 MHz antenna
- USB/UART interface
- Python monitoring dashboard
The shore station receives LoRa packets from boat units and forwards structured data to the Python dashboard for real-time monitoring and event logging.

Working Principle
1. The boat unit obtains the vessel's latitude and longitude.
2. The ESP32 evaluates the vessel position against a polyline representing the IMBL.
3. The perpendicular distance to each IMBL segment is calculated.
4. The minimum distance is selected as the vessel's effective boundary distance.
5. The system determines the vessel's safety state.
6. A green, yellow, or red LED indicates the current state.
7. When the vessel enters the critical zone, the buzzer is activated.
8. Boundary-alert information is transmitted through the 433 MHz LoRa link.
9. An SOS button press generates an SOS packet independently of the boundary state.
10. The shore station receives the packet and forwards structured JSON data to the PC.
11. The Python dashboard displays the received information and records events in a CSV log.
Geofence Logic

The IMBL is represented as a sequence of geographic line segments.

For the current vessel position, the system calculates the perpendicular distance to each segment and selects the minimum distance:
Vessel GPS Position
        ↓
Calculate distance to each IMBL segment
        ↓
Find minimum distance
        ↓
Determine safety state
        ↓
┌─────────────────────────────┐
│ Distance > 5 km             │ → SAFE
│ 2 km < Distance ≤ 5 km      │ → WARNING
│ Distance ≤ 2 km              │ → ALERT
└─────────────────────────────┘

The alert threshold is set at 2 km, while the warning threshold is set at 5 km.

Communication
The boat and shore units use SX1278 Ra-02 LoRa transceivers operating at 433 MHz.
LoRa is used because it provides long-range, low-power communication suitable for transmitting small safety and status packets without depending on cellular infrastructure.
The system supports transmission of:
- Boundary alerts
- SOS events
- GPS coordinates
- Vessel identifiers
- Safety status information

Hardware

Boat Unit
- ESP32-WROOM-32
- SX1278 Ra-02 LoRa transceiver
- 433 MHz antenna
- NEO-6M / compatible GPS interface
- Green LED
- Yellow LED
- Red LED
- Active buzzer
- SOS push button
- DC-DC buck converter
- 12 V boat battery / power source

Shore Unit
- ESP32-WROOM-32
- SX1278 Ra-02 LoRa transceiver
- 433 MHz antenna
- USB-to-UART interface
- 12 V DC adapter
- DC-DC buck converter
- PC for dashboard operation

Software

Embedded Firmware
The ESP32 firmware handles:
- GPS position processing
- IMBL geofence calculation
- Safety-state determination
- LED control
- Buzzer control
- SOS detection
- LoRa packet transmission and reception
- Serial communication

Shore Dashboard
The Python dashboard provides:
- Real-time boat monitoring
- Boundary status display
- Alert indication
- SOS event indication
- GPS information
- Event logging
- CSV-based data storage

Testing and Validation
The prototype was validated through:
- Bench integration testing
- Simulated GPS trajectory testing
- IMBL warning-threshold testing
- IMBL alert-threshold testing
- SOS transmission testing
- LoRa link-quality testing
- Shore dashboard integration testing
- Continuous-operation testing

The geofence algorithm triggered within approximately 20 m of the configured threshold under simulation.
The complete boundary-alert sequence executed within approximately 30 ms of the 2 km threshold being crossed.
LoRa testing at 433 MHz with SF10 achieved less than 2% packet loss at 500 m, with a measured 34 dB link-budget margin.
The integrated system operated continuously for two hours without firmware crashes, watchdog resets, or memory exceptions.
Project Status

This project represents a functional embedded prototype validated through bench and simulated testing.
The current implementation demonstrates the core boundary-detection, local-alert, LoRa communication, SOS, and shore-monitoring functions.
GPS input is currently validated through firmware-level simulation, while the firmware architecture supports connection of a real GPS module through the designated UART interface.

Limitations
The current prototype does not include:
- Marine-grade waterproof enclosure
- Solar charging system
- Custom PCB
- OLED display
- Voice alert system
- Open-water field validation
- Production-grade vessel hardware integration
These are considered future development areas.

Future Scope
- Integrate a real NEO-6M/NEO-M8N GPS receiver
- Conduct open-water field testing
- Develop a marine-grade waterproof enclosure
- Design a custom PCB
- Add solar-powered charging
- Improve LoRa network scalability
- Add OLED or other local display options
- Introduce voice-based alerts
- Improve dashboard visualization
- Develop a production-ready compact boat device
- Expand the system for larger fleets and multiple shore gateways.

Technology Stack

Hardware
- ESP32-WROOM-32
- SX1278 Ra-02
- 433 MHz LoRa
- GPS
- LEDs
- Active buzzer
- Push button
  
Firmware
- Embedded C/C++
- ESP32
- LoRa communication
- UART
  
Software
- Python
- Serial communication
- JSON
- CSV logging

Project Objective
The primary objective of Kadal Kaavalan is to demonstrate a low-cost, standalone, embedded safety system capable of warning fishermen before they approach the IMBL, while providing a communication link to a shore station for monitoring and event logging.

Authors
Tillai Natarajan S
Sundara Mahalingam S

Department of Electronics and Communication Engineering
Chennai Institute of Technology (Autonomous)
Affiliated to Anna University, Chennai

License
This project is developed as an academic Project Based Learning (PBL) project.
