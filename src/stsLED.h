/**
 * @file    stsLED.h
 * @author  Jeff Skarda (mapleiotsolutions@gmail.com)
 * @brief   All functions that are used to handle configuration
 * @details The library will read from EEPROM to set and update various configuration parameters
 * @version 0.1
 * @date    2022-09-01
 * 
 * Version History:
 * 0.1 - Initial realease 
 * 
 */

//Include a standard header guard
#ifndef __STSLED_H
#define __STSLED_H

//Include standard header files from libraries:
#include <arduino.h>
#include <ArduinoLog.h>

//Include application specific header files
#include "pinout.h"

//Macros(#define) to swap out during pre-processing (use sparingly)
#define LED stsLED::instance()
#define offBrightness 20

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * From global application setup you must call:
 * stsLED::instance().setup();
 * 
 * From global application loop you must call:
 * stsLED::instance().loop();
 */
class stsLED {
public:
    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use stsLED::instance() to instantiate the singleton.
     */
    static stsLED &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use stsLED::instance().setup();
     */
    void setup(uint8_t pin1);

    /**
     * @brief Perform application loop operations; call this from global application loop()
     * 
     * You typically use stsLED::instance().loop();
     */
    void loop();

    /**
     * @brief Will blink the LED the specified pattern for a specified number of times and ramp rate
     * 
     * You typically use stsLED::instance().flashPttrn());
     */
    void flash(uint16_t onDuration, uint16_t offDuration=0, uint16_t pauseDuration=0, uint16_t delay=0, uint8_t pulses=1, uint8_t count=1, uint8_t bulbSimRamp=0, uint8_t flashOffBrightness=offBrightness);
    
    /**
     * @brief Turn the sts LED on at full brightness (255)
     * 
     * You typically use stsLED::instance().on();
     */
    void on();
   
    /**
     * @brief Turn the sts LED off to the specified off Brightness
     * 
     * You typically use stsLED::instance().off;
     */
    void off(uint8_t bulbSimRamp = 0, uint8_t offOffBrightness = 0);
    
    /**
     * @brief Turn the sts LED on to a specified brightness (0-255)
     * 
     * You typically use stsLED::instance().pwm;
     */
    void pwm(uint8_t brightness);
    
    /**
     * @brief Determine if the blinking pattern is done. Used to determine if able to sleep or not or if the fault code is done being annunciated before power cycling
     * 
     * You typically use stsLED::instance().isDone;
     */
    bool isDone();

protected:
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use stsLED::instance() to instantiate the singleton.
     */
    stsLED();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~stsLED();

    /**
     * This class is a singleton and cannot be copied
     */
    stsLED(const stsLED&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    stsLED& operator=(const stsLED&) = delete;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static stsLED *_instance;

    byte     _state = 0;
    uint8_t oldState;
    uint8_t      _pin1;
    uint32_t _onDuration;
    uint32_t _offDuration;
    uint32_t _pauseDuration;
    uint8_t  _count;
    uint8_t  _pulses;
    uint8_t  _delay;
    uint8_t  _pulseCnt = 0;
    uint8_t  _seqCnt = 0;
    uint8_t  _brightness;
    uint8_t  _flashBrightness = 20;
    uint8_t  _flashOffBrightness = 0;
    uint8_t  _offBrightness = 0;
    uint8_t  _offOffBrightness = 0;
    uint8_t  _bulbSimRamp;
    uint8_t  _offBulbSimRamp;
    uint32_t _previousMillis = 0;
    uint32_t _previousFlashRampMillis = 0;
    uint32_t _previousOffRampMillis = 0;
    bool     _start;
    bool     _up;
    bool     _down;
};
#endif  /* __STSLED_H */