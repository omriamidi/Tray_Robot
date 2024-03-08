#include <Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;

const byte buttonPin0 = 0; 
const byte buttonPin1 = 1;
const byte buttonPin2 = 2;
const byte buttonPin3 = 3;
int animationToPlay = 0;


void setup() {
  Serial.begin(9600);
  myservo1.attach(9);
  myservo2.attach(5);
  myservo3.attach(6);

  pinMode(buttonPin0, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin0), selectDance, FALLING);
  pinMode(buttonPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), selectSpeedControl, FALLING);
  pinMode(buttonPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), selectMixerDance, FALLING);
  pinMode(buttonPin3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin3), selectBootProcess, FALLING);
}

void loop() {
    switch (animationToPlay) {
    case 1:
      dance();
      break;
    case 2:
      speedControl();
      break;
    case 3: 
      mixerDance();
      break;
    case 4:  
      bootProcess();
      break;
  }  
  animationToPlay = 0;
  goTo(0,0,0);
}

void selectDance() {
  animationToPlay = 1;
}

void selectSpeedControl() {
  animationToPlay = 2;
}

void selectMixerDance() {
  animationToPlay = 3;
}

void selectBootProcess() {
  animationToPlay = 4;
}

void bootProcess() {
  Serial.println("Playing bootProcess gesture");
  for (int i = 0; i < 7; i++) {
    goTo(90, 90, 90);
    delay(500);
    goTo(0, 0, 0);
    delay(500);
  }
}

void speedControl() {
  Serial.println("Playing speedControl gesture");
  for (int pos = 0; pos <= 90; pos += 1) {
    goTo(pos, pos, pos);
    delay(20);
  }
  delay(100);
  for (int pos = 90; pos >= 0; pos -= 1) {
    goTo(pos, pos, pos);
    delay(20);
  }
}

void mixerDance() {
Serial.println("Playing mixerDance gesture");
int pos = 70;
  for (int i = 0; i < 3; i++) {
    goTo(0, pos, pos);
    delay(300);
    goTo(pos, 0, pos);
    delay(300);
    goTo(pos, pos, 0);
    delay(300);
  }
  // goTo(0,0,0);
  //   delay(300);

}

void dance() {
  Serial.println("Playing dance gesture");
  int degrees = 45;
  int time = 3;
  int position = 0;

  for (int i = position; i <= degrees; i += 1) {
    goTo(i, 0, 0);
    delay(time);
  }
  for (int i = position; i <= degrees; i += 1) {
    goTo(0, i, 0);
    delay(time);
  }
  for (int i = position; i <= degrees; i += 1) {
    goTo(0, 0, i);
    delay(time);
  }
}


void goTo(int one, int two, int three) {
  myservo1.write(one);
  myservo2.write(two);
  myservo3.write(three);
  // Serial.print(one);
  // Serial.print(two);
  // Serial.println(three);

  //delay(20);
}
