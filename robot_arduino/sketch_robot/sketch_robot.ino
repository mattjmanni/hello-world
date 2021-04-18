/*
 * Arduino slave talks to Raspberry Pi
 * 
 * Pi tells Arduino what to do, and the Arduino controls the wheels.
 * Arduino talks to L298Ns to drive the wheels
 */

// Define a type to handle a wheel
struct Wheel {
  int IN1;
  int IN2;
  int EN;
};

// We have 4 wheels
const Wheel FL = {7, 6, 9};
const Wheel FR = {5, 4, 10};
const Wheel BL = {3, 2, 11};
const Wheel BR = {8, 13, 12};

// controls
const int STOP = 0;
const int FORWARD = 8;
const int BACKWARD = 5;
const int LEFT = 4;
const int RIGHT = 6;
const int ACCELERATE = 3;
const int BRAKE = 1;
const int MIN_SPEED = 127;
const int MAX_SPEED = 255;
const int DELTA_SPEED = 16;

// speed
int speed = MIN_SPEED;

void setup() {
  // set-up serial for comms to Pi
  Serial.begin(9600);
  
  // Set-up the four wheels
  setupWheel(FL);
  setupWheel(FR);
  setupWheel(BL);
  setupWheel(BR);
}

void loop() {
  // loop around reading from serial and driving the operations
  if (Serial.available() > 0) {
    // read incoming string
    int cmd = Serial.read();
    
    moveRobot(cmd);
  }
}

// Configure pins to handle a wheel
void setupWheel(Wheel wheel) {
  pinMode(wheel.IN1, OUTPUT);
  pinMode(wheel.IN2, OUTPUT);
  pinMode(wheel.EN, OUTPUT);
  wheelDirection(wheel, STOP);
}

// Set the wheel direction
void wheelDirection(Wheel wheel, int direction) {
  switch (direction) {
    case FORWARD:
      digitalWrite(wheel.IN1, HIGH);
      digitalWrite(wheel.IN2, LOW);
      break;
    case BACKWARD:
      digitalWrite(wheel.IN1, LOW);
      digitalWrite(wheel.IN2, HIGH);
      break;
     case STOP:
      // STOP
      digitalWrite(wheel.IN1, LOW);
      digitalWrite(wheel.IN2, LOW);
      wheelSpeed(wheel);
  }
}

// Set the wheel speed
void wheelSpeed(Wheel wheel) {
  analogWrite(wheel.EN, speed);
}

// move robot
void moveRobot(int cmd) {
  switch (cmd) {
    case FORWARD:
      wheelDirection(FL, FORWARD);
      wheelDirection(FR, FORWARD);
      wheelDirection(BL, FORWARD);
      wheelDirection(BR, FORWARD);
      break;
    case BACKWARD:
      wheelDirection(FL, BACKWARD);
      wheelDirection(FR, BACKWARD);
      wheelDirection(BL, BACKWARD);
      wheelDirection(BR, BACKWARD);
      break;
    case LEFT:
      wheelDirection(FL, BACKWARD);
      wheelDirection(FR, FORWARD);
      wheelDirection(BL, BACKWARD);
      wheelDirection(BR, FORWARD);
      break;
    case RIGHT:
      wheelDirection(FL, FORWARD);
      wheelDirection(FR, BACKWARD);
      wheelDirection(BL, FORWARD);
      wheelDirection(BR, BACKWARD);
      break;
    case ACCELERATE:
      if (speed < MAX_SPEED) {
        speed += DELTA_SPEED;
        wheelSpeed(FL);
        wheelSpeed(FR);
        wheelSpeed(BL);
        wheelSpeed(BR);
      }
      break;
    case BRAKE:
      if (speed > MIN_SPEED) {
        speed -= DELTA_SPEED;
        wheelSpeed(FL);
        wheelSpeed(FR);
        wheelSpeed(BL);
        wheelSpeed(BR);
      }
      break;
    case STOP:
      speed = MIN_SPEED;
      wheelDirection(FL, STOP);
      wheelDirection(FR, STOP);
      wheelDirection(BL, STOP);
      wheelDirection(BR, STOP);
  }
}
