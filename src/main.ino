/*
 * ESP8266 Ultrasonic Gesture Space Game (Optimized for Low Latency)
 * Board: NodeMCU ESP8266
 * Display: OLED 128x64 SSD1306 (I2C)
 * Sensor: HC-SR04
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// U8g2 Initialization for SSD1306 128x64 I2C
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ D1, /* data=*/ D2);

// Ultrasonic Pins
#define TRIG_PIN D5
#define ECHO_PIN D6
// Timeout for pulseIn: 25000us is approx 4 meters. Prevents long blocking if no echo.
#define SENSOR_TIMEOUT 25000 

// Game States
enum State { GUIDE, MENU, GAME, GAMEOVER };
State currentState = GUIDE;

// Menu Logic
int menuIndex = 0; 
unsigned long selectionStartTime = 0;
bool isSelecting = false;
const float SELECT_HOLD_TIME = 2500.0; 

// Game Variables
int rocketX = 64;
struct Asteroid {
  int x;
  float y;
  float speed;
  bool active;
};
Asteroid asteroids[4];
int score = 0;
unsigned long lastUpdate = 0;
float difficultyMultiplier = 1.0;
unsigned long gameStartTime = 0;

// Optimized distance reading
float readDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // pulseIn with timeout to prevent blocking more than ~25ms
  long duration = pulseIn(ECHO_PIN, HIGH, SENSOR_TIMEOUT);
  if (duration == 0) return 400.0; // Assume max distance if no echo
  
  return duration * 0.034 / 2;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  u8g2.begin();
  // Boost I2C speed to 400kHz for faster screen updates
  u8g2.setBusClock(400000); 
  
  for(int i=0; i<4; i++) {
    asteroids[i].active = false;
  }
}

void drawRocket(int x, int y) {
  u8g2.drawTriangle(x, y, x-4, y+8, x+4, y+8);
  u8g2.drawBox(x-2, y+8, 4, 2);
}

void runGuide() {
  static int frame = 0;
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(10, 20, "HOW TO PLAY");
  
  if (frame < 50) u8g2.drawStr(5, 40, "< 15cm: MOVE LEFT");
  else if (frame < 100) u8g2.drawStr(5, 40, "HOLD HAND: SELECT");
  else if (frame < 150) u8g2.drawStr(5, 40, "> 25cm: MOVE RIGHT");
  
  u8g2.drawFrame(10, 55, 108, 6);
  u8g2.drawBox(10, 55, (frame % 150) * 108 / 150, 6);
  
  u8g2.sendBuffer();
  frame++;
  if (frame > 200) currentState = MENU;
}

void runMenu() {
  float dist = readDistance();
  
  // High-response navigation
  if (dist > 0 && dist < 15) menuIndex = 0;
  else if (dist > 25) menuIndex = 1;
  
  if (dist > 0) {
    if (!isSelecting) {
      isSelecting = true;
      selectionStartTime = millis();
    }
    
    if (millis() - selectionStartTime >= SELECT_HOLD_TIME) {
      if (menuIndex == 0) {
        currentState = GAME;
        score = 0;
        difficultyMultiplier = 1.0;
        gameStartTime = millis();
        for(int i=0; i<4; i++) asteroids[i].active = false;
      } else {
        currentState = GUIDE;
      }
      isSelecting = false;
    }
  } else {
    isSelecting = false;
  }

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(25, 15, "MAIN MENU");
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(35, 35, "START GAME");
  u8g2.drawStr(35, 50, "USER GUIDE");
  u8g2.drawDisc(25, (menuIndex == 0 ? 32 : 47), 3);
  
  if (isSelecting) {
    int progress = (millis() - selectionStartTime) * 128 / SELECT_HOLD_TIME;
    u8g2.drawBox(0, 60, progress, 4);
  }
  u8g2.sendBuffer();
}

void runGame() {
  float dist = readDistance();
  
  // Real-time responsive movement
  if (dist > 0 && dist < 15) rocketX -= 4; // Increased sensitivity
  else if (dist > 25) rocketX += 4;
  
  if (rocketX < 5) rocketX = 5;
  if (rocketX > 123) rocketX = 123;

  difficultyMultiplier = 1.0 + (float)(millis() - gameStartTime) / 10000.0;
  if(difficultyMultiplier > 5.0) difficultyMultiplier = 5.0;
  
  // Higher frame rate for logic (no artificial delays)
  for(int i=0; i<4; i++) {
    if (!asteroids[i].active && random(0, 40) == 1) {
      asteroids[i].active = true;
      asteroids[i].x = random(5, 120);
      asteroids[i].y = 0;
      asteroids[i].speed = (0.5 + (float)random(0, 50)/100.0) * difficultyMultiplier;
    }
    
    if (asteroids[i].active) {
      asteroids[i].y += asteroids[i].speed;
      if (asteroids[i].y > 64) asteroids[i].active = false, score++;
      
      if (abs(asteroids[i].x - rocketX) < 6 && abs((int)asteroids[i].y - 54) < 6) {
        currentState = GAMEOVER;
        selectionStartTime = millis(); 
      }
    }
  }
  
  u8g2.clearBuffer();
  drawRocket(rocketX, 54);
  for(int i=0; i<4; i++) {
    if (asteroids[i].active) u8g2.drawDisc(asteroids[i].x, (int)asteroids[i].y, 3);
  }
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setCursor(0, 10); u8g2.print("S:"); u8g2.print(score);
  u8g2.setCursor(80, 10); u8g2.print("x"); u8g2.print(difficultyMultiplier, 1);
  u8g2.sendBuffer();
}

void runGameOver() {
  float dist = readDistance();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr);
  u8g2.drawStr(10, 30, "GAME OVER");
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setCursor(30, 45); u8g2.print("SCORE: "); u8g2.print(score);
  u8g2.drawStr(10, 58, "HOLD HAND TO EXIT");
  
  if (dist > 0) {
    unsigned long heldTime = millis() - selectionStartTime;
    u8g2.drawBox(0, 60, (heldTime * 128 / SELECT_HOLD_TIME), 4);
    if (heldTime >= SELECT_HOLD_TIME) currentState = MENU;
  } else {
    selectionStartTime = millis();
  }
  u8g2.sendBuffer();
}

void loop() {
  switch(currentState) {
    case GUIDE: runGuide(); break;
    case MENU: runMenu(); break;
    case GAME: runGame(); break;
    case GAMEOVER: runGameOver(); break;
  }
}
