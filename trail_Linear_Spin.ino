// version 3 explanation -
// In this version it was decided that there would be a rotary movement performed by one motor 
// and a linear movement performed by a second motor.
// In addition, to realistically illustrate a "serve" motion, I created a motion in which both motors work simultaneously. 
// That is, rotary and linear movement at the same time.

#include <ESP32Servo.h>
#include <ServoEasing.hpp>

#define SERVO_PIN 13
#define PIN_ENA  19 // The ESP8266 pin connected to the EN1 pin L2
#define PIN_IN1  5 // The ESP8266 pin connected to the IN1 pin L298N
#define PIN_IN2  4 // The ESP8266 pin connected to the IN2 pin L298N

ServoEasing myServo;

unsigned long lastDirectionChange = 0;
bool forward = true;
int directionInterval = 5000; // This defines how long it moves in each direction 
bool iMove = false;
int direction = 1;
unsigned long lastServoChange = 0;
int servoInterval = 50; // This defines how long it moves in each direction 

void setup() {
  myServo.attach(SERVO_PIN, 45); 
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  Serial.begin(115200);
  myServo.setEasingType(EASE_CUBIC_IN_OUT); // EASE_LINEAR is default
}

void loop() {
  // Serial.println(myServo.getCurrentAngle());
  spin();
  linear();
}

void spin() {
  if (millis() - lastServoChange >= servoInterval) {
     lastServoChange = millis();
      if(iMove == false)
      {
        myServo.startEaseTo(45, 40);  // Non blocking call
        iMove = true;
        return;
      }
      int currentAngle = myServo.getCurrentAngle();
      if(currentAngle == 135)
      {
      Serial.println("moving to 45");
        myServo.startEaseTo(45, 40);  // Non blocking call
        return;
      }
      if(currentAngle == 45 ){
      Serial.println("moving to 135");
        myServo.startEaseTo(135, 40);  // Non blocking call
        return;
      }
  }
}

void linear() {

  if (millis() - lastDirectionChange >= directionInterval) { // Check if it's time to change direction
    forward = !forward;
    lastDirectionChange = millis(); // Record the time of direction change
  }

  if (forward) {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
  } else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
  }

  analogWrite(PIN_ENA, 255);
}

// what's next?
// In the fourth version I'm working on now, the control of the robot's movement (linear and rotary movement) will be via the web by wifi.
// After version 4 is finished - we will start experiments with participants in the laboratory.
