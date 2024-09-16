#include "stsLED.h"

stsLED *stsLED::_instance;

// [static]
stsLED &stsLED::instance() {
    if (!_instance) {
        _instance = new stsLED();
    }
    return *_instance;
}

stsLED::stsLED() {
}

stsLED::~stsLED() {
}

void stsLED::setup(uint8_t pin1) {
    _pin1 = pin1;
    pinMode(_pin1, OUTPUT); // This is already done in the Pinout.cpp
}

void stsLED::loop() {
    // Put your code to run during the application thread loop here

    uint32_t currentMillis = millis();

    if(_state != oldState){
        Log.traceln("transition from: %i to: %i", oldState, _state);
        oldState = _state;
    }

    // State machine
    switch (_state) {
        case 0: //---- Step 0 (do nothing and leave the LED in whatever state it had previoiusly)
            _previousMillis = currentMillis;
            return;
            break;

        case 1: //---- Step 1 (Start delay for first pass - LED is OFF)
            if (currentMillis - _previousMillis >= _delay) {
                _state = 2;
            }
            break;
    
        case 2: //---- Step 2 (LED on)
            _up = true;
            _down = false;
            _pulseCnt ++; // Increase loop counter
            _previousMillis = currentMillis;
            _state = 3;
            _start = true;
            break;
            
        case 3: //---- Step 3 (ON duration)
            if (currentMillis - _previousMillis >= _onDuration) {
                _state = 4;
                _start = false;
            }
            break;
            
        case 4: //---- Step 4 (LED off)
            _down = true;
            _up = false;
            _previousMillis = currentMillis;
            _state = 5;
            break;
            
        case 5: //---- Step 5 (OFF duration)
            if (currentMillis - _previousMillis >= _offDuration) {
                _state = 6;
            }
            break;
            
        case 6: //---- Step 6 (Flash sequence finished?)
            if (_pulseCnt >= _pulses) {
                _pulseCnt = 0;
                _previousMillis = currentMillis;
                
                _seqCnt++;  // Increment the number of times the sequence ran
                //Determine if we ran the sequence the specified number of times. if _count = 0 then it's run indefinitly.
                if(_seqCnt >= _count && _count != 0){
                    _state = 0; //The Sequence is finished and we are finished with the number of times we want to run the sequence.
                }
                else{
                    _state = 7; //The Sequence is finished but we didn't finish the number of times we want to run the sequence.
                }
                
            } else {
                _state = 2;
            }
            break;
            
        case 7: //---- Step 7 (Pause duration)
            if (currentMillis - _previousMillis >= _pauseDuration) {
                _state = 2;
            }
            break;
    } // End of state machine
    
    if (_bulbSimRamp > 0) {
        // Ramp brightness
        if (micros() - _previousFlashRampMillis >= _bulbSimRamp) {
            _previousFlashRampMillis = micros();
            if (_up && _flashBrightness < 255) {
                _flashBrightness ++;
            }
            if (_flashBrightness == 255 || _down) _up = false;
            
            if (_down && _flashBrightness > _flashOffBrightness) {
                _flashBrightness --;
            }
            if (_flashBrightness == _flashOffBrightness) _down = false;
        }
    }
    else {
        // Change brightness immediately
        if (_up) {
            _flashBrightness = 255;
            _up = false;
        }
        if (_down) {
            _flashBrightness = _flashOffBrightness;
            _down = false;
        }
    }

    // Write brightness
    analogWrite(_pin1, _flashBrightness);
}


// Flash a Pattern function ************************************************************
void stsLED::flash(uint16_t onDuration, uint16_t offDuration, uint16_t pauseDuration, uint16_t delay, uint8_t pulses, uint8_t count, uint8_t bulbSimRamp, uint8_t flashOffBrightness) {
    _onDuration = onDuration;
    _offDuration = offDuration;
    _pauseDuration = pauseDuration;
    _delay = delay;
    _bulbSimRamp = bulbSimRamp;
    _pulses = pulses;
    _count = count;
    _flashOffBrightness = flashOffBrightness;
    _pulseCnt = 0;
    _seqCnt = 0;
    _previousMillis = millis();
    if(_delay == 0){
        _state = 2;
    }
    else{
        _state = 1;
    }

    loop();
}

// On function ************************************************************
void stsLED::on() {
    _state = 0;
    analogWrite(_pin1, 255);
}

// Off function ************************************************************
void stsLED::off(uint8_t bulbSimRamp, uint8_t offOffBrightness) {
    _state = 0;
    _offBulbSimRamp = bulbSimRamp;
    _offOffBrightness = offOffBrightness;

    if (_offBulbSimRamp > 0) {
        // Ramp brightness
        if (micros() - _previousOffRampMillis >= _offBulbSimRamp) {
            _previousOffRampMillis = micros();
            if (_offBrightness > _offOffBrightness) _offBrightness --;
        }
    }
    else _offBrightness = _offOffBrightness; // Change brightness immediately
    
    // Write brightness
    analogWrite(_pin1, _offBrightness);
}

// PWM function ************************************************************
void stsLED::pwm(uint8_t brightness) {
    _state = 0;
    _brightness = brightness;
    analogWrite(_pin1, _brightness);
}

bool stsLED::isDone(){
    if(_state == 0){
        return true;
    }
    else{
        return false;
    }
}
