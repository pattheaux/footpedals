// Program used to test the USB Joystick object on the 
// Arduino Leonardo or Arduino Micro.
//
// Matthew Heironimus
// 2015-03-28 - Original Version
// 2015-11-18 - Updated to use the new Joystick library 
//              written for Arduino IDE Version 1.6.6 and
//              above.
// 2016-05-13   Updated to use new dynamic Joystick library
//              that can be customized.
//------------------------------------------------------------

#include "Joystick.h"

// Create Joystick
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  0, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering
//Joystick_ Joystick;//Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,0,0,true,true);

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

const unsigned long gcCycleDelta = 1000;
const unsigned long gcAnalogDelta = 25;
const unsigned long gcButtonDelta = 500;
unsigned long gNextTime = 0;
unsigned int gCurrentStep = 0;


void testXYAxis(unsigned int currentStep)
{
  int xAxis;
  int yAxis;
  
  if (currentStep < 256)
  {
    xAxis = currentStep - 127;
    yAxis = -127;
    //Joystick.setXAxis(xAxis);
    Joystick.setYAxis(yAxis);
  } 
  else if (currentStep < 512)
  {
    yAxis = currentStep - 256 - 127;
    Joystick.setYAxis(yAxis);
  }
  else if (currentStep < 768)
  {
    xAxis = 128 - (currentStep - 512);
    Joystick.setXAxis(xAxis);
  }
  else if (currentStep < 1024)
  {
    yAxis = 128 - (currentStep - 768);
    Joystick.setYAxis(yAxis);
  }
  else if (currentStep < 1024 + 128)
  {
    xAxis = currentStep - 1024 - 127;
    Joystick.setXAxis(xAxis);
    Joystick.setYAxis(xAxis);
  }
}

void testZAxis(unsigned int currentStep)
{
  if (currentStep < 128)
  {
    Joystick.setZAxis(-currentStep);
  } 
  else if (currentStep < 256 + 128)
  {
    Joystick.setZAxis(currentStep - 128 - 127);
  } 
  else if (currentStep < 256 + 128 + 127)
  {
    Joystick.setZAxis(127 - (currentStep - 383));
  } 
}

void testHatSwitch(unsigned int currentStep)
{
  if (currentStep < 8)
  {
    Joystick.setHatSwitch(0, currentStep * 45);
  }
  else if (currentStep == 8)
  {
    Joystick.setHatSwitch(0, -1);
  }
  else if (currentStep < 17)
  {
    Joystick.setHatSwitch(1, (currentStep - 9) * 45);
  }
  else if (currentStep == 17)
  {
    Joystick.setHatSwitch(1, -1);
  }
  else if (currentStep == 18)
  {
    Joystick.setHatSwitch(0, 0);
    Joystick.setHatSwitch(1, 0);
  }
  else if (currentStep < 27)
  {
    Joystick.setHatSwitch(0, (currentStep - 18) * 45);
    Joystick.setHatSwitch(1, (8 - (currentStep - 18)) * 45);
  }
  else if (currentStep == 27)
  {
    Joystick.setHatSwitch(0, -1);
    Joystick.setHatSwitch(1, -1);
  }
}

void testThrottleRudder(unsigned int value)
{
  Joystick.setThrottle(value);
  Joystick.setRudder(value);
}

void testXYZAxisRotation(unsigned int degree)
{
  Joystick.setRxAxis(degree);
  Joystick.setRyAxis(degree);
  Joystick.setRzAxis(degree * 2);
}

void setup() {

  // Set Range Values
  Joystick.setAcceleratorRange(0, 600);
  Joystick.setBrakeRange(0, 600);

  Joystick.setThrottleRange(-600, 600);

  Joystick.setXAxisRange(0,600);
  Joystick.setYAxisRange(0,600);

  /*
  Joystick.setZAxisRange(-127, 127);
  Joystick.setRxAxisRange(0, 360);
  Joystick.setRyAxisRange(360, 0);
  Joystick.setRzAxisRange(0, 720);
  Joystick.setThrottleRange(0, 255);
  Joystick.setRudderRange(255, 0);
  */
  
  if (testAutoSendMode)
  {
    Joystick.begin();
  }
  else
  {
    Joystick.begin(false);
  }
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(9600);
    Serial.println("pedals");
}

void loop() {
/*
  // System Disabled
  if (digitalRead(A0) != 0)
  {
    // Turn indicator light off.
    digitalWrite(LED_BUILTIN, 0);
    return;
  }
  */

  // Turn indicator light on.
  digitalWrite(LED_BUILTIN, 1);
  
  if (millis() >= gNextTime)
  {
    int aval = analogRead(A0) - 200;
    int bval = analogRead(A1) - 200;

    if(aval < 20) {
      aval = 0;
    } else if(aval > 600) {
      aval = 600;
    }
    
    if(bval < 20) {
      bval = 0;
    } else if(bval > 600) {
      bval = 600;
    }

    int val = aval;
    if(bval > aval) {
      val = -bval;
    }
    
    Serial.print(aval);
    Serial.print(" ");
    Serial.print(bval);
    Serial.print(" ");
    Serial.println(val);

    //Joystick.setAccelerator(val);
    Joystick.setXAxis(aval);
    //Joystick.setBrake(bval);
    Joystick.setYAxis(bval);
    
    //Joystick.setThrottle(val);
    
    gNextTime = millis() + gcAnalogDelta;

    if (testAutoSendMode == false) {
      Joystick.sendState();
    }
    
    return;

   
  }
}
