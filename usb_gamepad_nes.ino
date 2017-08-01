/*
 *  
 *  
 *    Source: https://www.allaboutcircuits.com/projects/nes-controller-interface-with-an-arduino-uno/
 *  
 *  
 */


#include <Joystick.h>

const int A_BUTTON         = 0;
const int B_BUTTON         = 1;
const int SELECT_BUTTON    = 2;
const int START_BUTTON     = 3;
const int UP_BUTTON        = 4;
const int DOWN_BUTTON      = 5;
const int LEFT_BUTTON      = 6;
const int RIGHT_BUTTON     = 7;
//  Pro Micro Digital IOs: 2-10, 14-16
const int clk = 5;  
const int latch = 6;
const int din = 7;

int latch_data_delay = 10;
int data_in_delay = 100;
int loop_delay = 10 ;

String button_name[8] = { "A", "B", "SELECT", "START", "UP", "DOWN", "LEFT", "RIGHT" };

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_GAMEPAD,
  4,                      // Button Count
  0,                      // Hat Switch Count
  true, true, false,      // X and Y, but no Z Axis
  false, false, false,    // No Rx, Ry, or Rz
  false, false,           // No rudder or throttle
  false, false, false);   // No accelerator, brake, or steering
  
void toggle_clk(){
  // keep clock high so a low to high transition gets the next value
   digitalWrite(clk,LOW);
   digitalWrite(clk,HIGH);
}

void latch_data() {
  digitalWrite(latch,HIGH);
  digitalWrite(latch,LOW);
}

byte read_in_nes_data(){
  int controllerDataIn = 255;
  
  latch_data();
  for (int index = 0; index < 8; index++){
    if (digitalRead(din) == LOW){
      Serial.print("\tBUTTON ");
      Serial.print(button_name[index]);
      Serial.println(" was pressed");
    }
    toggle_clk();
    delay(10);
  }  
  return controllerDataIn;
}

void sendDataOverUsb(){

}



void setup() {
  pinMode(clk, OUTPUT);
  pinMode(din, INPUT);
  pinMode(latch, OUTPUT);

  // Initialize latch and clk
  digitalWrite(latch, LOW);
  digitalWrite(clk, HIGH);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
}

void loop() {
  read_in_nes_data();
  delay(loop_delay);
}


