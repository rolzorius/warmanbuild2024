// Library inclusions
#include <Servo.h>    // Servo Library
#include <AFMotor.h>  // Motor

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

AF_Stepper stepper1(200, 2);  // Stepper for the turret rotation. This defines stepper1(steps/rev, channel#). i.e. 200steps/rev, channel 1.
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
  actuator.setSpeed(0);
}

void lower_ball1() {  // Lower arm for ball 1.
  actuator.run(BACKWARD);
  actuator.setSpeed(255);
  delay(7000); 
  actuator.setSpeed(0);  // THIS NEEDS TESTING! ---------------------------- TEST ME ----------------------------
}
void lower_ball2() {  // Lower arm for ball 2.
  actuator.run(BACKWARD);
  actuator.setSpeed(255);
  delay(7000); 
  actuator.setSpeed(0);  // THIS NEEDS TESTING! ---------------------------- TEST ME ----------------------------
}
void lower_ball3() {  // Lower arm for ball 3.
  actuator.run(BACKWARD);
  actuator.setSpeed(255);
  delay(7000); 
  actuator.setSpeed(0);  // THIS NEEDS TESTING! ---------------------------- TEST ME ----------------------------
}
void lift_ball1() {
  actuator.run(FORWARD);
  actuator.setSpeed(255);
  delay(3200);
  actuator.setSpeed(0);
}
void lift_ball2() { 
  actuator.run(FORWARD);
  actuator.setSpeed(255);
  delay(3200);
  actuator.setSpeed(0);
}
void lift_ball3() { 
  actuator.run(FORWARD);
  actuator.setSpeed(255);
  delay(3200);
  actuator.setSpeed(0);
}
void lower_hole1() {
  actuator.run(BACKWARD);
  actuator.setSpeed(255);
  delay(7000); 
  actuator.setSpeed(0);
}
void lower_hole2() {
  actuator.run(BACKWARD);
  actuator.setSpeed(255);
  delay(7000); 
  actuator.setSpeed(0);
}
void extend_1() {  // Full extend on arm.
  extender.run(FORWARD);
  extender.setSpeed(200);
  delay(750);
  extender.setSpeed(0);
}

void extend_2() {  // Full extend on arm.
  extender.run(FORWARD);
  extender.setSpeed(200);
  delay(100);  // CHANGE MEEEEEEEE
  extender.setSpeed(0);
}
void extend_hole() {
    extender.run(FORWARD);
  extender.setSpeed(200);
  delay(100);  // CHANGE MEEEEEEEE
  extender.setSpeed(0);
}

void extend_3() {  // Full extend on arm.
  extender.run(FORWARD);
  extender.setSpeed(200);
  delay(750);  // CHANGE MEEEEEEEE
  extender.setSpeed(0);
}

void retract_full() {  // Full retract on arm.
  extender.run(BACKWARD);
  extender.setSpeed(175);
  delay(750);
  extender.setSpeed(0);
}

void open() {
  servo1.write(90);
}

void close() {
  servo1.write(0);
}

void rotate_ball1() {                       // Rotate turret Counter-clockwise
  stepper1.step(300, FORWARD, INTERLEAVE);  // Change (this, FORWARD, INTERLEAVE) if you want to use this movement function for a specific task.
}

void rotate_ball2() {
  stepper1.step(300, BACKWARD, INTERLEAVE);  // Change (this, BACKWARD, INTERLEAVE) if you want to use this function for a specific purpose.
}
void rotate_ball3() {
  stepper1.step(300, BACKWARD, INTERLEAVE);  // Change (this, BACKWARD, INTERLEAVE) if you want to use this function for a specific purpose.
}
void rotate_hole1() {
  stepper1.step(300, BACKWARD, INTERLEAVE);  // Change (this, BACKWARD, INTERLEAVE) if you want to use this function for a specific purpose.
}
void rotate_hole2() {
  stepper1.step(300, BACKWARD, INTERLEAVE);  // Change (this, BACKWARD, INTERLEAVE) if you want to use this function for a specific purpose.
}
// Main loop

void loop() {

  switch (task) {
    case 1:
      // Task 1 - Idling, waiting for button push.
      while (digitalRead(BUTTON) == 1) {
        Serial.print("Waiting for instructions.");
        if (digitalRead(BUTTON) == 0) {
          Serial.print("Switching to task 2!");
          task = 2;
        }
      }


    case 2:
      // Task 2 - Turn turret so arm is in on intercept course with first ball. Raise arm to relevant height and extend.
      elevateS2H();
      extend_1();
      open();
      rotate_ball1();
      lower_ball1();
      close();
     // delay(1000);
      lift_ball1();
      retract_full();
      open();
      task = 3;


        //stepper2.step(100, FORWARD, SINGLE);



        break;
      // do we need curly brakets to encompass all of the case so one after case 2: and one after break 

    case 3:
      // Task 3
      rotate_ball2();
      extend_1();
      lower_ball2();
      close();
      retract_full();
      rotate_hole1();
      lower_hole1(); //to the hole
      extend_hole();
      open();

      task = 4;

 

  
      

        break;
      

    case 4:
      // Task 4
      elevateS2H(); // use this to lift arm to above horizontal 
      rotate_ball3(); // from hole position so needs to be opposite way 
      extend_3();
      lower_ball3();
      close();
      lift_ball3();
      retract_full();
      rotate_hole2();
      lower_hole2();
      extend_hole(); // should be same distance
      open();     

      break;
  }
}
