/*
  Algobrain.h - Library for coding the Algobrain board by Algobrix.
  Created by Tzah Mazuz, August 25, 2019.
  Released into the public domain.
*/
#pragma once
#ifndef Algobrain_h
#define Algobrain_h
#include "Arduino.h"

#include "../Adafruit_NeoPixel/Adafruit_NeoPixel.h"
#ifndef ADAFRUIT_NEOPIXEL_H
#error Sketch must include Adafruit_NeoPixel.h
#endif

// UI Pins
#define PlayButtonPin   14
#define PlayLedPin      4
#define GreenPowerLed   8
#define RedPowerLed     21
#define PowerMeterPin   A6
#define R1              20000.0 // resistance of R1 (20K)
#define R2              10000.0 // resistance of R2 (10K)
// LED Pins
#define Led_A           19
#define Led_B           18
  // LED Colors
  #define ColorRed      1
  #define ColorGreen    2
  #define ColorBlue     3
// Motor Pins
//  Directions:
#define Motor_A_Dir     10
#define Motor_B_Dir     2
#define Motor_C_Dir     6
#define Motor_Sleep_Pin 13
//  PWM Pins:
#define Motor_A_PWM     9
#define Motor_B_PWM     3
#define Motor_C_PWM     5
// Motor Select:
#define Motor_A         1
#define Motor_B         2
#define Motor_C         3
#define CW              0
#define CCW             1
// Sensors Pins
#define Sensor_A        A3
#define Sensor_B        A2
// MP3 Pins
#define MP3TXPIN        22
#define MP3STATEXPIN    23
#define SETTXLOW() PORTE =  PORTE & B11111110
#define SETTXHIGH() PORTE = PORTE | B00000001
// UI Behaviour
#define LedBlinkTimeMillis          500
#define PowerCheckOnceEveryMillis   2000
#define ShutdownMillis              1500 // 1.5 Seconds delay for a manual shutdown
#define SleepTimeout                600000 // 10 Minutes = 600000 Milliseconds
#define PowerMax                    900	// Full Power - Green LED
#define PowerHalf                   750 // Half Power - Red LED Blink
#define PowerBlink                  650	// No Power - Red LED

// FOR LOGS
#define LOG
// System Defintions
#ifdef LOG
#define RX_MESSAGE "[RRX]"
#define TX_MESSAGE "[RTX]"
#define SYSTEM_STATUS "[RSS]"
#define LOG_BEGIN(...) Serial.begin(__VA_ARGS__)
#define LOG_FLUSH(...) Serial.flush(__VA_ARGS__)
#define LOG_PRINT(...)  Serial.print (__VA_ARGS__)
#define LOG_PRINTLN(...)  Serial.println (__VA_ARGS__)
#define LOG_PRINTLN_PREFIX(x,y) \ 
Serial.print(F(x)); \ 
Serial.println(y);
#else
#define LOG_BEGIN(...)
#define LOG_FLUSH(...)
#define LOG_PRINT(...)
#define LOG_PRINTLN(...)
#define LOG_PRINTLN_PREFIX(x,y)
#define LOG_VERBOSE_PRINT(...)  
#define LOG_VERBOSE_PRINTLN(...)
#endif

struct UIButtons {
    byte buttonPin;
    byte ledPin;
    boolean buttonState;
    boolean buttonLastState;
    unsigned long buttonLastStateMillis;
    boolean ledState;
    boolean blinkState;
    unsigned long ledStateMillis;
};

class Algobrain
{
  public:
    Algobrain();
    void runUI();
    void setup();
    boolean playButtonListener(void (*)() = nullptr);
    // Sensors
    float getSensor(byte sensorId);
    // LEDs - NeoPixel
    void setLed(byte ledId, uint8_t r, uint8_t g, uint8_t b);
    void setLed(byte ledId, uint8_t color);
    // Motors
    void setMotorSpeed(byte motorId, byte pwm);
    void setMotorDir(byte motorId, boolean dir);
    void setMotor(byte motorId, boolean dir, byte pwm);

    

  private:
    // Variables :
      // Power related
      unsigned long lastPowerCheckMillis = 0;
      unsigned long lastPowerLedToggleMillis = 0;
      boolean isPowerLedBlinking = false;
      boolean powerLedState = false;
      // UI and Listener
      boolean playButtonRead;
      boolean isPlaying = false;
      int batteryLevel = 0;
    // Structs :
      UIButtons playButton;
    // Classes :
      Adafruit_NeoPixel LedA = Adafruit_NeoPixel(1, Led_A, NEO_GRB + NEO_KHZ800);
      Adafruit_NeoPixel LedB = Adafruit_NeoPixel(1, Led_B, NEO_GRB + NEO_KHZ800);
    // Functions :
      // Setup
      void setupUI();
      void setupPeripherals();
      void setupPowerLeds();
      void setupPlayButton();
      void setupSensors();
      void setupLeds();
      void setupMotors();
      // UI and Listener
      void changeButtonLedState(UIButtons btn, boolean newState);
      void changeButtonLedBlinkState(UIButtons btn, boolean newState);
      int getBattery();
      void checkPower(boolean checkPowerNow = false);
      void checkIsPlaying();
      void shutdown();
};
#endif
