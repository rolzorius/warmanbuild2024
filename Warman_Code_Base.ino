// Library inclusions
#include <Servo.h>         // Servo Library
#include <AFMotor.h> // Motor

// Pins
#define BUTTON 34

// Global Variables
int pos = 0;
int dir = 0;
int cycle = 4;
int task = 1;  // variable used for switch case

unsigned long startMillis;           //
unsigned long currentMillis;         // Count milliseconds once a timer starts.
const unsigned long period = 10000;  // This is 10 seconds in milliseconds.
unsigned long delayedstart;

AF_Stepper stepper1(200, 2);    // Stepper for the turret rotation. This defines stepper1(steps/rev, channel#). i.e. 200steps/rev, channel 1.
AF_DCMotor extender(1);
AF_DCMotor actuator(2);
Servo servo1;

// Define booleans
bool button_released = true;


// Initial setup code - RUN ONCE
void setup() {
  Serial.begin(115200);
  servo1.attach(9);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalRead(BUTTON);
  actuator.setSpeed(255);
  actuator.run(RELEASE);

}

// Movement functions
void elevateS2H() {
  actuator.run(FORWARD);
  actuator.setSpeed(255);
  delay(13200);
  actuator.setspeed(0);
}

void lower_ball1() {            // Lower arm for ball 1. 
  actuator.run(BACKWARD);
  actuator.setSpeed(255);
  delay(7000);                // THIS NEEDS TESTING! ---------------------------- TEST ME ----------------------------
}

void extend_full() {          // Full extend on arm.
  extender.run(FORWARD);
  extender.setSpeed(200);
  delay(750);
  extender.setSpeed(0);
  }

void retract_full() {         // Full retract on arm.
  extender.run(BACKWARD);
  extender.setSpeed(175);
  delay(750);
  extender.setSpeed(0);
}
  

void rotate_ball1() {           // Rotate turret Counter-clockwise
  stepper1.step(300, FORWARD, INTERLEAVE);    // Change (this, FORWARD, INTERLEAVE) if you want to use this movement function for a specific task.
}

void rotate_ball2(){
  stepper1.step(300, BACKWARD, INTERLEAVE);   // Change (this, BACKWARD, INTERLEAVE) if you want to use this function for a specific purpose. 
}


// Main loop

void loop() {

  switch (task) {
    case 1:
      // Task 1 - Idling, waiting for button push.
      while (digitalRead(BUTTON) == 1){
      Serial.print("Waiting for instructions.");
      if (digitalRead(BUTTON) == 0) {
        Serial.print("Switching to task 2!");
        task = 2;
      }
      }
  

  case 2:
    // Task 2 - Turn turret so arm is in on intercept course with first ball. Raise arm to relevant height and extend.
    
    rotate_ball1();
    //stepper2.step(100, FORWARD, SINGLE);
    
    
    
    
    
    while (digitalRead(BUTTON) == 1) {  // while button is NOT pressed,
      Serial.print("Executing task 2.")
        digitalWrite(LED, HIGH);
      delay(4000);  // is this seconds or milliseconds?
      digitalWrite(LED, LOW);
      delay(4000);  // is this seconds or milliseconds?
      if (digitalRead(BUTTON) == 0) {
        task = 3;
        Serial.print("Button pressed!");
      }
      break;
    }

  case 3:
    // Task 3 
    while (digitalRead(BUTTON) == 1) {  // while button is NOT pressed,
      Serial.print("Executing task 3.")
        goForwards();
      motor_power = 128  // Motor power at 50%
        analogWrite(ENA, motor_power);
      startMillis = millis();
      if (digitalRead(BUTTON) == 0 || currentMillis >= period) {  // please check this OR condition >> Time specifically
        Serial.print("Button pressed or 10s passed!");
        breakStop();
        task = 4;
      }
      break;
    }

  case 4:
  // Task 4
    Serial.print("Waiting 2 seconds!")
      stepper.setAcceleration(50);
    startMillis = millis();
    currentMillis = millis();
    if (2000 <= currentMillis <= 8000) {
      Serial.print("Executing task 4!")
        goBackwards();
      motor_power = 204  // Motor power at 80%
        analogWrite(ENA, motor_power);
      while (currentMillis >= 3000) {
        for (int i = 0; i < 8 * stepsPerRevolution; i++) {
          digitalWrite(dirPin, HIGH);
          digitalWrite(stepPin, HIGH);
          delayMilliseconds(2000);
          digitalWrite(stepPin, LOW);
          delayMilliseconds(2000);
        }
      }
      if (currentMillis >= 8000) {
        breakStop();
        task = 5;
      }
    }

    break;

  case 5:
    // Task 5 
    Serial.print("Executing task 5!")
      currentMillis = millis();
    if (currentMillis >= 2000) {
      Serial.print("Executing task 5!");
      stepper.setAcceleration(3000);
      stepper.setSpeed(200);
      for (int i = 0; i < 6 * stepsPerRevolution; i++) {
        digitalWrite(dirPin, LOW);
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(2000);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(2000);
      }
      digitalWrite(stepPin, LOW);
      break;
    }
}
}
