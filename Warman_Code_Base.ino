// Library inclusions
#include <Servo.h>         // Servo Library
#include <AccelStepper.h>  // Stepper Library

// Pins
#define SERVOPIN 10
#define BUTTON 2
Servo myservo;
#define IN1 3
#define IN2 4
#define ENA 9
#define dirPin_1 5    // Direction pin STEPPER 1
#define stepPin_1 6   // Step pin STEPPER 1
#define dirPin_2 X    // Direction pin STEPPER 2 - need to put pin variable in
#define stepPin_2 Y   // Step pin STEPPER 2 - need to put pin variable in
#define motorInterfaceType 1

// Global Variables
int pos = 0;
int dir = 0;
int cycle = 4;
int task = 1;  // variable used for switch case

int value = 0;
int motor_power = 0;  // Send this to serial monitor to confirm power as expected
int fromLow = 0;      // Lowest value seen by potentiometer.
int fromHigh = 1023;  // Highest value seen by potentiometer.
int toLow = 50;       // Lowest value where the motor starts to spin. May need adjusting
int toHigh = 255;     // Highest value you can send to motor controller.

unsigned long startMillis;           //
unsigned long currentMillis;         // Count milliseconds once a timer starts.
const unsigned long period = 10000;  // This is 10 seconds in milliseconds.
unsigned long delayedstart;

#define stepsPerRevolution = 2048;
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

// Define booleans
bool button_released = true;

// Initial setup code - RUN ONCE
void setup() {
  Serial.begin(115200);
  myservo.attach(SERVOPIN);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalRead(BUTTON);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  stepper.setMaxSpeed(500);
  stepper.setAcceleration(500)
}

// Movement functions
void goForwards() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void goBackwards() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void coastStop() {  // Slowly decreases speed in whatever direction it's currently going.
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
}

void breakStop() {  // Quickly stops, regardless of direction.
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}


// Main loop

void loop() {

  switch (task) {

    case 1:  // Task 1 - Idling, waiting for button push.
      while (digitalRead(BUTTON) == 1){
      Serial.print("Waiting for instructions.");
      if (digitalRead(BUTTON) == 0) {    // if button pressed, switch to task 2. 
        Serial.print("Switching to task 2!");
        task = 2;
      }
      }
  

  case 2:
    // Task 2 - 
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
