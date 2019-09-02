#include <Adafruit_NeoPixel.h>
#include <Algobrain.h>

int num = 0;
// Returns an algobrain class instance
Algobrain algobrain;

void setup() {
  // Equals to --> Serial.Begin(115200);
  LOG_BEGIN(115200);
  // setups the algobrain (sets the board pins and resets everything)
  algobrain.setup();
}

void loop() {
  // runUI - Takes care of the board User Interface, checks battery power and gives info with the power LED
  algobrain.runUI();
  // This playButtonListener gets a function (function can be null) to execute whenever the play button has been pressed
  // This listener also returns a boolean whether the button was pressed --> true if pressed.
  // Usecase 1: Send someFunction to execute.
  boolean isPlayPressed = algobrain.playButtonListener(someFunction);
  // Usecase 2: After pressed do something...
  if(isPlayPressed) {
    someFunctionToo();
  }
}

// Quick Guide on using the algobrain functions.

// LED's :
/* setLed --> Sets a led to some given color
 *            Gets Unique LedId (Led_A, Led_B)           
 *            Gets a color of 3 values or a Color DEFINE (r, g, b)<-->(0-255) \ (Color DEFINE)<-->(ColorRed, ColorGreen, ColorBlue)
 */
 
// Motors :
/* setMotor --> Sets a motor to direction and speed
 *              Gets Unique MotorId (Motor_A, Motor_B, Motor_C)
 *              Gets Motor Direction (CW, CCW)
 *              Gets Motor Speed (0-255)
 */
// Variations of the setMotor are setMotorSpeed(motorId, speedValue) and setMotorDir(motorId, direction)

/* getSensor -->  returns the high duty cycle value of the sensor
 *                Gets Unique SensorId(Sensor_A, Sensor_B) 
 */
              
void someFunction() {
  // Flag variation (like flipping a coin)
  switch (num % 2) {
    case 0:
      algobrain.setLed(Led_A, 255, 0, 0);
      algobrain.setMotor(Motor_A, CW, 255);
      break;
    case 1:
      algobrain.setLed(Led_A, ColorGreen);
      algobrain.setMotor(Motor_A, CCW, 255);
      break;
  }
  num += 1; // Flips the coin...
  float sensorVal = algobrain.getSensor(Sensor_A);
  LOG_PRINTLN(sensorVal); // Prints the sensor returned value
}

void someFunctionToo() {
  LOG_PRINTLN(F("Hello World!"));
}
