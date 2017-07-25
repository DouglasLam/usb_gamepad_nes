#include <Joystick.h>

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  4,                      // Button Count
  0,                      // Hat Switch Count
  true, true, false,      // X and Y, but no Z Axis
  false, false, false,    // No Rx, Ry, or Rz
  false, false,           // No rudder or throttle
  false, false, false);   // No accelerator, brake, or steering
//  Micro Pro Digital IOs: 2-10, 14-16
int clk = 2;  // is this right?  I'm using DIO#, shouldn't this be pin#?
int din = 3;  // is this right?  I'm using DIO#, shouldn't this be pin#?
int latch = 4;  // is this right?  I'm using DIO#, shouldn't this be pin#?
int latch_data_delay = 10;
int data_in_delay = 10;
int loop_delay = 10;
int controllerDataIn[8];
int currentButtonState[8];  
  
void toggle_clk(int delay_time=10){
  // keep clock high so a low to high transition gets the next value
   digitalWrite(clk,0);
   delay(delay_time);
   digitalWrite(clk,1);
}

void latch_data() {
  digitalWrite(latch,1);
  delay(10);
  digitalWrite(latch,0);
}

void sendDataOverUsb(){
  // [0-7]:  A, B, SELECT, START, UP, DOWN, LEFT, RIGHT
  Serial.print("A\t\t" + controllerDataIn[0]);
  Serial.print("B\t\t" + controllerDataIn[1]);
  Serial.print("SELECT\t" + controllerDataIn[2]);  
  Serial.print("START\t" + controllerDataIn[3]);
  Serial.print("UP\t\t" + controllerDataIn[4]);
  Serial.print("DOWN\t" + controllerDataIn[5]);
  Serial.print("LEFT\t" + controllerDataIn[6]);
  Serial.print("RIGHT\t" + controllerDataIn[7]);
  Serial.print("*****************************\n\n");

}



void setup() {
  pinMode(clk, OUTPUT);
  pinMode(din, INPUT);
  pinMode(latch, OUTPUT);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
}

void loop() {
  digitalWrite(clk,1);    // reset clock
  latch_data;
  delay(latch_data_delay);
  
  
  // Read pin values
  for (int index = 0; index < 8; index++)
  {
    toggle_clk();
    controllerDataIn[index] = digitalRead(din);
  }
  sendDataOverUsb();
  delay(loop_delay);
}


