#include "0_headers.h"
#include "A_Colors.h"
#include "B_Movement.h"
#include "C_Sensors.h"

// Analog sensor for LDR and IR.
MeRGBLed RGBled(0, 30);
MePort ir_adapter(PORT_3);   //Analog sensors - reads LDR
MePort ldr_adapter(PORT_4);  //Connected to LDR and LED Circuits


bool finished = false;
void setup() {
  //setup the outputs for the colour sensor
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(LDR, INPUT);
  led.setpin(13);

  //begin serial communication

  Serial.begin(9600);
  Serial.println("setup complete");
  //setBalance();  //calibration of color sensor - disabled for final run
}


void loop() {
  
  if (!finished) {
    correctionFunction();
    if (reached_paper()) {
      stopMotor();
      int cur_colour;
      cur_colour = detect_colour(); // returns an index which corresponds to the global array global_colours[]

      if (cur_colour == 0) {          //white
        celebrate();
        finished = true;
      } else if (cur_colour == 1) {  //red
        turnLeft();
      } else if (cur_colour == 2) {  //blue
        doubleRightTurn();
      } else if (cur_colour == 3) {  //green
        turnRight();
      } else if (cur_colour == 4) {  //pink
        doubleLeftTurn();
      } else {  //orange
        uTurn();
      }
    }
  }
  
}