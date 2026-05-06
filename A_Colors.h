#include "0_headers.h"
float white[] = {260, 290, 290}; //white
float red[] = {240, 130, 115}; //red
float blue[] = {115, 190, 220}; //blue
float green[] = {115, 200, 160}; //green
float pink[] = {250, 230, 230}; //pink - smh still inaccurate:p
float orange[] = {250, 175, 140}; //orange
float* global_colours[] = {
  white,
  red,
  blue,
  green,
  pink,
  orange
};
float ledArray[] = {0, 0, 0 };
float whiteArray[] = {810, 491, 497.6 };      
float blackArray[] = {684.8, 193.8, 204.8};
float greyDiff[] = {125.2, 297.2, 292.8};
char colourStr[3][5] = { "R = ", "G = ", "B = " }; // used for testing and callibration purpose; displaying of RGB values
MeRGBLed led(0, 30);

// Displays the color the robot detected.
void show_Color(int c){
  int colorArray[6][3] = {
    {255,255,255},  // white
    {255,0,0},      // red
    {0,0,255},      // blue
    {0,255,0},      // green
    {255,130,150},  // pink
    {255,165,0},    // orange
  };
  led.setColor(colorArray[c][0],colorArray[c][1],colorArray[c][2]); 
  led.show(); 
  delay(500);
}

// CALCULATES RELATIVE DISTANCE BETWEEN DETECTED COLOR AND PROPOSED COLOR TO DETERMINE WHICH COLOR IT CLOSELY RESEMBLES
float get_error(float* colour_array, float* ref_colour) {   
    float x = pow((colour_array[0] - ref_colour[0]), 2);
    float y = pow((colour_array[1] - ref_colour[1]), 2);
    float z = pow((colour_array[2] - ref_colour[2]), 2);
    return sqrt(x + y + z);
}

int get_colour(float* colour_array) {
    float min_error = 10000;
    int colour_idx = 10000;
    for (int i = 0; i < 6; i++) {
        float cur_error = get_error(colour_array, global_colours[i]);
        if (cur_error < min_error) {
            min_error = cur_error;
            colour_idx = i;
        }
    }
    show_Color(colour_idx);   
    return colour_idx;      // returns colour_idx that corresponds to one of the index of global_colours[]
}

float getAvgReading(int times) {
  //find the average reading for the requested number of times of scanning LDR
  float reading;
  float total = 0;
  //take the reading as many times as requested and add them up
  for (int i = 0; i < times; i++) {
    reading = analogRead(LDR);
    total = reading + total;
    delay(LDRWait);
  }
  //calculate the average and return it
  return total / times;
}

//Callibration of Colors - DISABLED IN FINAL RUN
void setBalance() {
  //set white balance
  Serial.println("Put White Sample For Calibration ...");
  delay(5000);  //delay for five seconds for getting sample ready
  //Check Indicator OFF during Calibration
  //scan the white sample.
  //go through one colour at a time, set the maximum reading for each colour -- red,
  //green and blue to the white array
  for (int i = 0; i <= 2; i++) {
    if (i == 0) {
      // RED Turns On 
      digitalWrite(S1, HIGH);
      digitalWrite(S2, HIGH);
      delay(RGBWait);
    }  
    else if (i == 1) {
      // GREEN 
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      delay(RGBWait);
    } else {
      // Blue
      digitalWrite(S1, LOW);
      digitalWrite(S2, HIGH);
      delay(RGBWait);
    }
    whiteArray[i] = getAvgReading(5); //scan 5 times and return the average
    // Turns off LEDs.
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);
    delay(RGBWait);
  }
  //done scanning white, time for the black sample.
  //set black balance
  Serial.println("Put Black Sample For Calibration ...");
  delay(5000);  //delay for five seconds for getting sample ready
  //go through one colour at a time, set the minimum reading for red, green and blue
  //to the black array
  for (int i = 0; i <= 2; i++) {
    if (i == 0) {
      // RED
      digitalWrite(S1, HIGH);
      digitalWrite(S2, HIGH);
      delay(RGBWait);
    }  //scan 5 times and return the average
    else if (i == 1) {
      //GREEN
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
      delay(RGBWait);
    } else {
      //BLUE
      digitalWrite(S1, LOW);
      digitalWrite(S2, HIGH);
      delay(RGBWait);
    }
    //scan 5 times and return the average
    blackArray[i] = getAvgReading(5);
    greyDiff[i] = whiteArray[i] - blackArray[i]; //the differnce between the maximum and the minimum gives the range
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);
    delay(1000);
  }

  for (int j = 0; j <= 2; j++) {
    Serial.println(whiteArray[j]);
    Serial.println(blackArray[j]);
    Serial.println((greyDiff[j]));
  }
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  delay(RGBWait);

  //delay another 5 seconds for getting ready colour objects
  Serial.println("Colour Sensor Is Ready.");
  delay(5000);
}


int detect_colour() {
  //turn on one colour at a time and LDR reads 5 times
  for (int c = 0; c <= 2; c++) {
    Serial.print(colourStr[c]);
    if (c == 0) {
      digitalWrite(S1, HIGH);
      digitalWrite(S2, HIGH);
    }  else if (c == 1) {
      digitalWrite(S1, HIGH);
      digitalWrite(S2, LOW);
    } else {
      digitalWrite(S1, LOW);
      digitalWrite(S2, HIGH);
    }
    delay(RGBWait);
    //get the average of 5 consecutive readings for the current colour and return an average
    ledArray[c] = getAvgReading(10);
    
    //the average reading returned minus the lowest value divided by the maximum
    //possible range, multiplied by 255 will give a value between 0-255, representing the
    //value for the current reflectivity (i.e. the colour LDR is exposed to)
    ledArray[c] = (ledArray[c] - blackArray[c]) / (greyDiff[c]) * 255;
    
    digitalWrite(S1, LOW);
    digitalWrite(S2, LOW);  //turn off the current LED colour
    delay(RGBWait);
    Serial.println(int(ledArray[c]));  //show the value for the current colour LED, which corresponds to either the R, G or B of the RGB code
  
  }
  return get_colour(ledArray);
}