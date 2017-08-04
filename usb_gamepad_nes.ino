/*
 *  
 *  
 *    Requires:   https://github.com/DouglasLam/ArduinoJoystickLibrary
 *    
 *    Reference:  https://www.allaboutcircuits.com/projects/nes-controller-interface-with-an-arduino-uno/
 *  
 */
#include <Joystick.h>

const int A_BUTTON        = 0;
const int B_BUTTON        = 1;
const int SELECT_BUTTON   = 2;
const int START_BUTTON    = 3;
const int UP_BUTTON       = 4;
const int DOWN_BUTTON     = 5;
const int LEFT_BUTTON     = 6;
const int RIGHT_BUTTON    = 7;
const int clk             = 5;  
const int latch           = 6;
const int din             = 7;
const int loop_delay      = 10;

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
  int data_in = 255;
  
  latch_data();
  for (int index = 0; index < 8; index++){
    if (digitalRead(din) == LOW){
      switch (index){
        case 0:  // A Button
          bitClear(data_in, A_BUTTON);
          break;
        case 1: // B Button
          bitClear(data_in, B_BUTTON);
          break;
        case 2: // SELECT Button
          bitClear(data_in, SELECT_BUTTON);
          break;
        case 3: // START Button
          bitClear(data_in, START_BUTTON);
          break;
        case 4: // UP Button
          bitClear(data_in, UP_BUTTON);
          break;
        case 5: // DOWN Button
          bitClear(data_in, DOWN_BUTTON);
          break;
        case 6: // LEFT Button
          bitClear(data_in, LEFT_BUTTON);
          break;     
        case 7: // RIGHT Button
          bitClear(data_in, RIGHT_BUTTON);
          break;
      }                     
      Serial.print("\tBUTTON ");
      Serial.print(button_name[index]);
      Serial.println(" was pressed");
    }
    toggle_clk();
  }  
  return data_in;
}

void send_data_over_usb(byte controller_data){
  //  D-Pad
  if (bitRead(controller_data, UP_BUTTON) == 0){Joystick.setYAxis(-1);}
  else if (bitRead(controller_data, DOWN_BUTTON) == 0){Joystick.setYAxis(1);}
  else{Joystick.setYAxis(0);}  
  if (bitRead(controller_data, RIGHT_BUTTON) == 0){Joystick.setXAxis(1);}
  else if (bitRead(controller_data, LEFT_BUTTON) == 0){Joystick.setXAxis(-1);}
  else{Joystick.setXAxis(0);}

  //  BUTTONS
  if (bitRead(controller_data, SELECT_BUTTON) == 0){ Joystick.setButton(0, 1);}
  else {Joystick.setButton(0,0);}
  if (bitRead(controller_data, START_BUTTON) == 0){ Joystick.setButton(1, 1);}
  else {Joystick.setButton(1,0);}
  if (bitRead(controller_data, B_BUTTON) == 0){ Joystick.setButton(2, 1);}
  else {Joystick.setButton(2,0);}
  if (bitRead(controller_data, A_BUTTON) == 0){ Joystick.setButton(3, 1);}
  else {Joystick.setButton(3,0);}
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
  byte controller_data = read_in_nes_data();
  send_data_over_usb(controller_data);
  delay(loop_delay);
}


