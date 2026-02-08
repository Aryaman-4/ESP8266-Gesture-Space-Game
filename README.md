# 🚀 ESP8266 Gesture-Controlled Space Game

A real-time **gesture-controlled space game** built using **ESP8266 (NodeMCU)**, **HC-SR04 ultrasonic sensor**, and an **OLED display**.  
The game uses hand distance as input — no buttons, no joystick.

---

## 🎮 Gameplay Controls

| Hand Distance | Action |
|--------------|--------|
| < 15 cm | Move rocket left |
| > 25 cm | Move rocket right |
| Hold hand | Menu select / exit |

---


![WhatsApp Image 2026-02-08 at 1 21 47 PM](https://github.com/user-attachments/assets/232ff8d6-7d2b-4161-a557-474a6245cff8)






## ✨ Features

- Gesture-based control using ultrasonic sensor
- OLED-based UI with menus and guide screen
- Real-time responsive movement
- Progressive difficulty system
- Optimized for low latency
- Fully embedded (no external controller)

---

## 🧠 How It Works

1. Ultrasonic sensor measures hand distance
2. ESP8266 processes distance values
3. Rocket movement is mapped to gestures
4. OLED renders game graphics in real time
---
## 🔧 Hardware Requirements
- ESP8266 NodeMCU
- HC-SR04 Ultrasonic Sensor
- OLED 128×64 SSD1306 (I2C)
- Jumper wires
- Breadboard
---
## 🛠 Software & Libraries

- Arduino IDE
- ESP8266 Board Package
- U8g2 Graphics Library
- Wire Library
---


Circuit Diagram

<img width="1280" height="720" alt="Untitled design" src="https://github.com/user-attachments/assets/eb4de616-bdf4-44db-800a-6cf74de0e84a" />

# 🔌 Pin Connections

## Ultrasonic Sensor (HC-SR04)

| Sensor Pin | ESP8266 Pin |
|-----------|------------|
| VCC | 5V |
| GND | GND |
| TRIG | D5 |
| ECHO | D6 |

---

## OLED Display (SSD1306 – I2C)

| OLED Pin | ESP8266 Pin |
|--------|-------------|
| VCC | 3.3V |
| GND | GND |
| SDA | D2 |
| SCL | D1 |

---

⚠️ Note: Use voltage-safe connections for the ECHO pin if required.






