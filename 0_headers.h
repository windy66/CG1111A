#include <MeMCore.h>
#define time 1000
#define RGBWait 500  //in milliseconds
#define LDRWait 5    //in milliseconds
#define LDR A2       //LDR sensor pin at A2  - RETURNS READINGS OF LDR   
#define IR A3        //IR detector pin at A3 RETURNS READINGS OF IR DETECTOR
#define S1 A0        // S1 and S2 are used to control the LEDs and IR Detector
#define S2 A1
#define ULTRASONIC 12         // pin number of ultrasonic sensor
#define TIMEOUT 2000          // the time duration the sensor will wait for the reflected ultrasonic pulses. 
#define SPEED_OF_SOUND 340
#define CORRECTION_SPEED 35   // used to adjust the motorspeed for both motors to prevent mBot from colliding with maze walls




