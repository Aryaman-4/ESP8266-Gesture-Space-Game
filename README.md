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




