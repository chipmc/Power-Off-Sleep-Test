/**
 * @file    Pinout.h
 * @author  Jeff Skarda (mapleiotsolutions@gmail.com)
 * @brief   All functions that are used to establish pin mapping and set pinout functionality 
 * @details 
 * @version 0.1
 * @date    2022-09-01
 * 
 * Version History:
 * 0.1 - Initial realease 
 * 
 */

//Include a standard header guard
#ifndef __PINOUT_H
#define __PINOUT_H

//Include standard header files from libraries:
#include <arduino.h>
#include <ArduinoLog.h>

//Include application specific header files
// ---NONE---

//Macros(#define) to swap out during pre-processing (use sparingly)
#define gpio pinout::instance()

/**********************************************************************************************************************
 * ******************************            Pinout Details                  ******************************************
 * 
 *
 * Replace ME - Add pinout documentation details here
 * 
***********************************************************************************************************************/

/**
 * This class is a singleton; you do not create one as a global, on the stack, or with new.
 * 
 * From global application setup you must call:
 * pinout::instance().setup();
 * 
 * From global application loop you must call:
 * pinout::instance().loop();
 */
class pinout {
public:
    /**
     * @brief Gets the singleton instance of this class, allocating it if necessary
     * 
     * Use pinout::instance() to instantiate the singleton.
     */
    static pinout &instance();

    /**
     * @brief Perform setup operations; call this from global application setup()
     * 
     * You typically use pinout::instance().setup();
     */
    void setup();

    /**
     * @brief Perform application loop operations; call this from global application loop()
     * 
     * You typically use pinout::instance().loop();
     */
    void loop();


protected:
    /**
     * @brief The constructor is protected because the class is a singleton
     * 
     * Use pinout::instance() to instantiate the singleton.
     */
    pinout();

    /**
     * @brief The destructor is protected because the class is a singleton and cannot be deleted
     */
    virtual ~pinout();

    /**
     * This class is a singleton and cannot be copied
     */
    pinout(const pinout&) = delete;

    /**
     * This class is a singleton and cannot be copied
     */
    pinout& operator=(const pinout&) = delete;

    /**
     * @brief Singleton instance of this class
     * 
     * The object pointer to this class is stored here. It's NULL at system boot.
     */
    static pinout *_instance;

public:
    //Define pins for the RFM9x:
    static const uint8_t RFM95_CS       = 4;
    static const uint8_t RFM95_RST      = 3;
    static const uint8_t RFM95_INT      = 2;
    static const uint8_t RFM95_DIO0     = A5;
    // //Define pins for the Sensors:
    // Analog Pins
    // A0
    // A1
    // A2  
    // A3      
    static const uint8_t BATTINT        = A4;
    // Analog pin A5 is used by the RFM95
    // Analog pin A6 / D8 / WAKE / PA06
    // Analog pin A7 / D9 / PA07
    // Analog pin A8 - A10 are not broken out
    static const uint8_t A11            = 25;        // This needs to be validated PB03 - Mislabeled on board as A6

    // Digital Pins
    static const uint8_t RX             = 0;
    static const uint8_t TX             = 1;
    // Digital pins 2-4 are used by the RFM95
    static const uint8_t STATUS         = 5;
    static const uint8_t I2C_EN         = 6;        // Enable pin for i2c sensors
    static const uint8_t I2C_INT        = 7;        // i2c sensors INT PIN
    static const uint8_t WAKE           = 8;
    static const uint8_t D9             = 9;
    static const uint8_t EN             = 10;       // PIR Sensor on Digital - Not Enable - D10
    static const uint8_t USER_SW        = 11;
    static const uint8_t LED_PWR        = 12;       // PIR Sensor on Digital - LED-PWR - D12
    static const uint8_t INT            = 13;       // PIR Sensor on Digital - INT - D13
    // Digital pins 14 is also PA02 which is A0
    // Digital pin 15 is also PA03 which is A1
    // Digital pin 16 is also PA09 which is A2
    // Digital pin 17 is also PA04 which is A3
    // Digital pin 18 is also PA05 which is A4
    // Digital pin 19 is also PA02 which is A5
    // SDA            = 20;
    // SCL            = 21;
    // MISO           = 22;
    // MOSI           = 23;
    // SCK            = 24;
    // ???            = 25;
    static const uint8_t CE             = 26; // PB27
    static const uint8_t D14            = 27; // D14 / PA28 - Need to figure this one out
    // ???            = 28;
    // ???            = 29;
    static const uint8_t Test1          = 30; // PB22 - Now connected to !CHG for charge detection
    static const uint8_t Test2          = 31; // PB23


    // These pins are used directly by the SAMD21
    // SWCLK / PA30 - Debugging - NA
    // SWDIO / PA31 - Debugging - NA
    // D+ / PA24 - NA
    // D- / PA25 - NA

};
#endif  /* __PINOUT_H */