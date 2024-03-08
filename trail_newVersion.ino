#include <Servo.h>

#define XVAL_PIN A0  
#define POTENTIOMETER_PIN A2 
#define BUTTON_PIN 0

int horizontalMove; // joystick x value
int spinMove; // potentiometer
int tiltMove; // joystick button

Servo tiltServo;
Servo spinServo;
Servo horizontalServo;

void setup() {
  Serial.begin(9600);

  pinMode(XVAL_PIN, INPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);

  tiltServo.attach(3);
  spinServo.attach(5);
  horizontalServo.attach(6);
}

void loop() {

  // reading all values
  spinMove = analogRead(POTENTIOMETER_PIN);
  spinMove = map(spinMove, 0, 1023, 0, 180);
  horizontalMove = analogRead(XVAL_PIN);
  horizontalMove = map(horizontalMove, 0, 1023, 0, 180);
  tiltMove = abs(digitalRead(BUTTON_PIN) - 1);
  tiltMove = map(tiltMove, 0, 1, 0, 90);

  // printing all values
  Serial.print(spinMove);
  Serial.print(", ");
  Serial.print(horizontalMove);
  Serial.print(", ");
  Serial.println(tiltMove); 

  //servos activation
  tiltServo.write(tiltMove);
  spinServo.write(spinMove);
  horizontalServo.write(horizontalMove);
  delay(100);
}


