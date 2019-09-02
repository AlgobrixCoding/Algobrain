/*
  Algobrain.cpp - Library for coding the Algobrain board by Algobrix.
  Created by Tzah Mazuz, August 25, 2019.
  Released into the public domain.
*/
#include "Algobrain.h"

Algobrain::Algobrain() {

}

void Algobrain::setup() {
    setupUI();
    setupPeripherals();
    LOG_PRINTLN_PREFIX(SYSTEM_STATUS, "Algobrain Library 1.0");
}

void Algobrain::setupUI() {
    setupPowerLeds();
    setupPlayButton();
}

void Algobrain::setupPeripherals() {
    setupSensors();
    setupLeds();
    setupMotors();
}

void Algobrain::setupPowerLeds() {
    // Default start up state : Green(ON) , Red(OFF).
    pinMode(GreenPowerLed, OUTPUT);
    pinMode(RedPowerLed, OUTPUT);
    digitalWrite(GreenPowerLed, HIGH);
    digitalWrite(RedPowerLed, LOW);
}

void Algobrain::setupPlayButton() {
    // Play Button Setup
    playButton.buttonPin = PlayButtonPin;
    playButton.ledPin = PlayLedPin;
    playButton.buttonState = true;
    playButton.buttonLastState = true;
    playButton.ledState = false;
    playButton.blinkState = false;
    playButton.buttonLastStateMillis = millis();
    pinMode(playButton.buttonPin, INPUT);
    pinMode(playButton.ledPin, OUTPUT);
}

void Algobrain::setupSensors() {
    pinMode(Sensor_A, INPUT);
    pinMode(Sensor_B, INPUT);
}

void Algobrain::setupLeds() {
    LedA.begin();
    LedB.begin();
    setLed(Led_A, 0, 0, 0);
    setLed(Led_B, 0, 0, 0);
}

void Algobrain::setupMotors() {
    pinMode(Motor_A_Dir, OUTPUT);
    pinMode(Motor_A_PWM, OUTPUT);
    pinMode(Motor_B_Dir, OUTPUT);
    pinMode(Motor_B_PWM, OUTPUT);
    pinMode(Motor_C_Dir, OUTPUT);
    pinMode(Motor_C_PWM, OUTPUT);
    pinMode(Motor_Sleep_Pin, OUTPUT);
    digitalWrite(Motor_Sleep_Pin, HIGH);
}

float Algobrain::getSensor(byte sensorId) {
    const unsigned long cycleTime = 2000; // 2 ms in microsecnds 500HZ Freq
    const unsigned long pulseTimeout = 4000;
    unsigned long pwm_value_H = 0;
    float DutyCycle = 0;
    pwm_value_H = pulseIn(sensorId, HIGH, pulseTimeout);
    if (pwm_value_H != 0)
        DutyCycle = (float(pwm_value_H) / float(cycleTime)) * 100.0;
    else if (digitalRead(sensorId))
        DutyCycle = 100;
    else
        DutyCycle = 0;
    return DutyCycle;
}

void Algobrain::setLed(byte ledId, uint8_t r, uint8_t g, uint8_t b) {
    switch(ledId) {
        case Led_A:
            LedA.setPixelColor(0, LedA.Color(r, g, b));
            LedA.show();
            break;
        case Led_B:
            LedB.setPixelColor(0, LedB.Color(r, g, b));
            LedB.show();
            break;
    }
}

void Algobrain::setLed(byte ledId, uint8_t color) {
    switch (color)
    {
    case ColorRed:
        setLed(ledId, 255, 0, 0);
        break;
    case ColorGreen:
        setLed(ledId, 0, 255, 0);
        break;
    case ColorBlue:
        setLed(ledId, 0, 0, 255);
        break;
    }
}

void Algobrain::setMotorSpeed(byte motorId, uint8_t pwm) {
    switch (motorId)
    {
    case Motor_A:
        analogWrite(Motor_A_PWM, pwm);
        break;
    case Motor_B:
        analogWrite(Motor_B_PWM, pwm);
        break;
    case Motor_C:
        analogWrite(Motor_C_PWM, pwm);
        break;
    }
}

void Algobrain::setMotorDir(byte motorId, boolean dir) {
    switch (motorId) 
    {
    case Motor_A:
        digitalWrite(Motor_A_Dir, dir);
        break;
    case Motor_B:
        digitalWrite(Motor_B_Dir, dir);
        break;
    case Motor_C:
        digitalWrite(Motor_C_Dir, dir);
        break;
    }
}

void Algobrain::setMotor(byte motorId, boolean dir, uint8_t pwm) {
    setMotorSpeed(motorId, pwm);
    setMotorDir(motorId, dir);
}

void Algobrain::changeButtonLedState(UIButtons btn, boolean newState) {
    btn.ledStateMillis = millis();
    btn.ledState = newState;
    digitalWrite(btn.ledPin, newState);
}

void Algobrain::changeButtonLedBlinkState(UIButtons btn, boolean newState) {
  btn.blinkState = newState;
  changeButtonLedState(btn, newState);
}

void Algobrain::checkPower(boolean checkPowerNow = false) {
 
  // Red Power LED Blinking State
  if (isPowerLedBlinking) {
	// Every 350 Millis
    if (lastPowerLedToggleMillis + 350 < millis()) {
		// State changes
		powerLedState = !powerLedState;
		lastPowerLedToggleMillis = millis();
		// Set the LED to the opposite state
		digitalWrite(RedPowerLed, powerLedState);
    }
  }

  // Read every 10 millis
  if ((millis()) % 10 == 0)  {
    batteryLevel = 0.9 * batteryLevel + 0.1 * analogRead(PowerMeterPin);
  }
  
  if (lastPowerCheckMillis + PowerCheckOnceEveryMillis < millis() || checkPowerNow) {
    
	int value = getBattery();
    
	// LOG_PRINT(F("voltage :"));
	// LOG_PRINTLN(String(vin));
	// LOG_PRINT(F("batteryLevel :"));
	// LOG_PRINTLN(String(batteryLevel));
	// LOG_PRINT(F("value :"));
	// LOG_PRINTLN(String(value));
    
    // Setting the LED
	if (value <= PowerBlink) {
      isPowerLedBlinking = true;
	  digitalWrite(RedPowerLed, HIGH);
	  digitalWrite(GreenPowerLed, LOW);
    } else if (value <= PowerHalf) {
      digitalWrite(RedPowerLed, HIGH);
	  digitalWrite(GreenPowerLed, LOW);	  
	  isPowerLedBlinking = false;
    } else {
      isPowerLedBlinking = false;
      digitalWrite(RedPowerLed, LOW);
	  digitalWrite(GreenPowerLed, HIGH);
    }
    // Set last check
    lastPowerCheckMillis = millis();
  }
}

int Algobrain::getBattery() {
    // Function that returns battery level
	float vout = 0.0;
    float vin = 0.0;
    vout = (batteryLevel * 5.0) / 1024.0;
    vin = vout / (R2 / (R1 + R2));
    int value = vin * 100; // Turn to int from float	
	if (vin == 0) {
		LOG_PRINTLN_PREFIX(SYSTEM_STATUS, "USB POWER");
	} else {
		LOG_PRINTLN(String(SYSTEM_STATUS) + "Battery Value " + (String)vin + "V");
	}
    LOG_FLUSH();
    return value;
}

void Algobrain::checkIsPlaying() {
    // if play button is blinking:
    if (playButton.blinkState) {
        if (playButton.ledStateMillis + LedBlinkTimeMillis <= millis()) {
        changeButtonLedState(playButton, !playButton.ledState);
        }
    }
}

void Algobrain::shutdown() {
  // Turn off all button LEDs
  changeButtonLedState(playButton, false);

  pinMode(playButton.buttonPin, OUTPUT);
  digitalWrite(playButton.buttonPin, LOW);
  while (1) {}
}

void Algobrain::runUI() {
    checkPower();
    checkIsPlaying();
}

boolean Algobrain::playButtonListener(void (*someFunction)() = nullptr) {
    // Handle Button Press !!!
    boolean isPlayButtonPressed = false;
    // Play Control - Act on release
    playButtonRead = digitalRead(playButton.buttonPin);
    // Play Button Pressed DOWN --> digitalRead is LOW \ 0
    if (!playButtonRead) {
        // Save the last state and its time
        playButton.buttonLastState = playButtonRead;
        playButton.buttonLastStateMillis = millis();   
    }
    // Play Button Released: Not Playing -> Start Playing
    else if (playButton.buttonLastState != playButtonRead) {
        isPlayButtonPressed = true;
        // Play when code available
        if(someFunction != nullptr) {
            (*someFunction)();
        }
        // Save the last state and its time
        playButton.buttonLastState = playButtonRead;
        playButton.buttonLastStateMillis = millis();
    }
    // Play Button NOT Released: Long Press -> Shut Down Brain
    else if ((!playButtonRead) && ((playButton.buttonLastStateMillis + ShutdownMillis) < millis())) {
        if (getBattery() > 20) {
            shutdown();
        }
    }

    // Timeout control - sleep after 10 minutes of inactivity
    if ((playButton.buttonLastStateMillis + SleepTimeout) < millis()) {
        if (getBattery() > 20) {
            shutdown();
        }
    }
    return isPlayButtonPressed;
}
