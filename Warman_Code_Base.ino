// Library inclusions
#include <Servo.h>         // Servo Library
#include <AFMotor.h> // Motor

// Pins
#define SERVOPIN 10
#define BUTTON 2
Servo myservo;
#define IN1 3
#define IN2 4
#define ENA 9
#define dirPin_1 5    // Direction pin STEPPER 1
#define stepPin_1 6   // Step pin STEPPER 1
#define dirPin_2 X    // Direction pin STEPPER 2
#define stepPin_2 Y   // Step pin STEPPER 2
#define motorInterfaceType 1
#define enc1 Z  // encoder for extend
#define enc2 XZ // encoder for drive

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
int enc1_count = digitalRead(enc1); // Encoder1 count >>> 20 = 1 full rotation

unsigned long startMillis;           //
unsigned long currentMillis;         // Count milliseconds once a timer starts.
const unsigned long period = 10000;  // This is 10 seconds in milliseconds.
unsigned long delayedstart;

#define stepsPerRevolution_1 = 200 // as per motor datasheet.
#define stepsPerRevolution_2 = 200 // as per motor datasheet.
AF_Stepper stepper1(200, 1);    // Stepper for the turret rotation. This defines stepper1(steps/rev, channel#). i.e. 200steps/rev, channel 1.
AF_Stepper stepper2(200, 2);    // Stepper for the arm elevation.

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
  stepper1.setSpeed(20);  // Use this to change stepper1 speed.
  stepper2.setSpeed(20);  // Use this to change stepp2 speed. REMEMBER; THE HIGHER THE SPEED THE LOWER THE TORQUE!
  pinMode(enc1, INPUT);
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

void extend_1() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  for (enc_1_count <= 20){
    analogWrite(XXX, 155);
  }
  

}

void rotate_CCW() {
  s
  digitalWrite(dirPin_1, LOW); // set direction CCW
  digitalWrite(stepPin_1, HIGH); // Step pin to high, causes rotation

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
    stepper1.step(100, FORWARD, SINGLE);
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
