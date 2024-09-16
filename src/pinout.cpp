#include "pinout.h"

pinout *pinout::_instance;

// [static]
pinout &pinout::instance() {
    if (!_instance) {
        _instance = new pinout();
    }
    return *_instance;
}

pinout::pinout() {
}

pinout::~pinout() {
}

void pinout::setup() {
  //Setup pins to be ins or outs. 
  pinMode(STATUS,OUTPUT);
  pinMode(RFM95_CS,OUTPUT);
  pinMode(RFM95_DIO0,INPUT_PULLUP);
  pinMode(RFM95_RST, OUTPUT);
  pinMode(WAKE, INPUT_PULLUP);
  pinMode(I2C_INT,INPUT_PULLDOWN);
  pinMode(I2C_EN,OUTPUT);                                   // Not sure if we can use this - Need to test as this might mess with the i2c bus
  digitalWrite(I2C_EN, HIGH);                               // Turns on the production module - change to LOW if we are using a pre-production module
  pinMode(BATTINT,INPUT_PULLUP);                            // Battery interrupt pin    
  pinMode(USER_SW,INPUT_PULLUP);                            // User switch pin
  
  //Establish a random seed based on the tank level pin
  randomSeed(analogRead(Test1));
}

void pinout::loop() {
    // Put your code to run during the application thread loop here
}
