#include "0_headers.h"

MeDCMotor leftMotor(M1);            // assigning leftMotor to port M1
MeDCMotor rightMotor(M2);      
uint8_t motorSpeed = 420;

double left_distance() {
  pinMode(ULTRASONIC, OUTPUT);
  digitalWrite(ULTRASONIC, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRASONIC, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC, LOW);
  pinMode(ULTRASONIC, INPUT);
  long duration = pulseIn(ULTRASONIC, HIGH, TIMEOUT);
  return (duration / 2.0 / 1000000 * SPEED_OF_SOUND * 100);
}

void moveForward() {
  // Code for moving forward for some short interval
  leftMotor.run(motorSpeed);
  rightMotor.run(-motorSpeed);
}

void moveSlightlyRight() {
    // for when mBot is too close to left wall
    leftMotor.run(motorSpeed - CORRECTION_SPEED);
    rightMotor.run(-motorSpeed - CORRECTION_SPEED);
}

void moveSlightlyLeft() {
    // for when mBot is close to right wall
    leftMotor.run(motorSpeed + CORRECTION_SPEED);
    rightMotor.run(-motorSpeed + CORRECTION_SPEED);
}

float getAvgIr(int times){
  float reading;
  float total = 0;
  for (int i = 0; i < times; i++) {
    reading = analogRead(IR);
    total = reading + total;
  }
  return total / times;
}

float ir_read(){
   // DETECTS AMBIENT IR
   digitalWrite(S1, HIGH);
   digitalWrite(S2, LOW);
   delay(1);
   float ambient_ir = getAvgIr(3);

   // DETECTS ACTUAL IR
   digitalWrite(S1, LOW);
   digitalWrite(S2, LOW);
   delay(1);
   float detected_ir = getAvgIr(3);
   
   Serial.println("RIGHT");
   Serial.println(detected_ir - ambient_ir);
   // RETURNS DIFFERENCE BETWEEN DETECTED IR AND AMBIENT IR
   return (detected_ir - ambient_ir);
}

void correctionFunction() {
    //Function used to prevent robot from bumping into maze walls
    const double WALL_THRESHOLD_CLOSE = 9;  // cm (too close to wall)
    double leftDist = left_distance(); // ultrasonic sensor on left
    Serial.println(leftDist);
    float ir_diff = ir_read();
  
    // If too close to left wall, steer right
    if (leftDist < WALL_THRESHOLD_CLOSE && leftDist != 0) {
        moveSlightlyRight();
    }

    // If too close to right wall, steer left
    else if (ir_diff < -100 ){
        moveSlightlyLeft();
    }
    // Else, keep going straight
    else {
        moveForward();
    }
   
}

void stopMotor() {
  // Code for stopping motor
  leftMotor.stop();
  rightMotor.stop();
}

void turnRight() {
  leftMotor.run(-motorSpeed); // Negative: wheel turns anticlockwise
  rightMotor.run(-motorSpeed); // Negative: wheel turns anticlockwise
  delay(450); // Keep turning right for this time duration
  leftMotor.stop(); // Stop left motor
  rightMotor.stop(); // Stop right motor
}

void turnLeft() {
  leftMotor.run(motorSpeed); // Positive: wheel turns clockwise
  rightMotor.run(motorSpeed); // Positive: wheel turns clockwise
  delay(450); // Keep turning left for this time duration
  leftMotor.stop(); // Stop left motor
  rightMotor.stop(); // Stop right motor
}

//add reached_paper function
void uTurn() {
  leftMotor.run(motorSpeed); // Positive: wheel turns clockwise
  rightMotor.run(motorSpeed); // Positive: wheel turns clockwise
  delay(900); // Keep turning left for this time duration - 180 degrees.
  leftMotor.stop(); // Stop left motor
  rightMotor.stop(); // Stop right motor
}

void doubleLeftTurn() {    // Code for double left turn
  //turn left
  turnLeft();
  // moving forward
  moveForward();
  // Delay for the mBot to move forward a bit
  delay(900);
  //turn left
  turnLeft();            
}

void doubleRightTurn() {  // Code for double right turn
  //turn right
  turnRight();
  // moving forward
  moveForward();
  // Delay for the mBot to move forward a bit
  delay(900);
  //turn right
  turnRight();          
}