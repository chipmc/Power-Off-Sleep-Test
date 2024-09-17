#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <ArduinoLog.h>
#include <AB1805_RK.h>
#include "pinout.h"
#include "stsLED.h"

#define IRQ_AB1805 1

AB1805 ab1805(Wire); // Class instance for the the AB1805 RTC

// put function declarations here:
void wakeUp_Timer();
void preSleep();
void postSleep();

// Variables - we do not have a time source
volatile uint8_t IRQ_Reason = 0; 										// 0 - Invalid, 1 - AB1805, 2 - RFM95 DIO0, 3 - RFM95 IRQ, 4 - User Switch, 5 - Sensor

void setup() {
	Serial.begin(115200);												      // Establish Serial connection if connected for debugging
	delay(2000);                                      // Let the serial connect

  // Log.begin(LOG_LEVEL_SILENT, &Serial);
	Log.begin(LOG_LEVEL_INFO, &Serial);

  gpio.setup(); 														        // Setup the pins

  LED.setup(gpio.STATUS);                           // Initialize the LED
  LED.on();

  ab1805.withFOUT(gpio.WAKE).setup();               // Set up the RTC with WAKE on pin 8

  // This is how to check if we did a deep power down (optional)
  AB1805::WakeReason wakeReason = ab1805.getWakeReason();  
  if (wakeReason == AB1805::WakeReason::DEEP_POWER_DOWN) {
    Log.infoln("Third Test - Success - Woke from DEEP_POWER_DOWN");
    Log.infoln("*********************************************");
  }
  else {
    Log.infoln("*********************************************");
	  Log.infoln("See Insights LoRa Node - Power Off Sleep Test");
  }

  LowPower.attachInterruptWakeup(gpio.WAKE, wakeUp_Timer, FALLING);   // Attach the interrupt

  { // First Test - Regular Sleep
    Log.infoln("First Test - sleep for 10 seconds");
    preSleep();
    LowPower.sleep(10000);                          // Sleep for 10 seconds
    postSleep();
    Log.infoln("Awoke from first test - pause for 5 seconds");
  }

  { // Second Test - Wake on interrupt
    Log.infoln("Second Test - sleep for 10 seconds & wake on clock");
    preSleep();                                            // Get the message out before sleeping
    ab1805.interruptCountdownTimer(10,false);        // 10 seconds - no clock set required
    LowPower.sleep(12000);                           // Set the alarm for 12 seconds as a backstop
    postSleep();

    if (IRQ_Reason == IRQ_AB1805) {
      Log.infoln("Woke on clock - success (WAKE pin = %s)", (digitalRead(gpio.WAKE)) ? "High":"Low");         // Woke by clock on WAKE pin
    }
    else {
      Log.infoln("Woke on timeout - failure (WAKE pin = %s)", (digitalRead(gpio.WAKE)) ? "High":"Low");       // Woke by the timeout
    }
    IRQ_Reason = 0;
  }

  { // Third Test - Deep Power Down Sleep - limited to 255 seconds
    time_t seconds = 10;
    time_t time_cv;
    uint8_t hundrths_cv;

    Log.infoln("Third Test - Power off sleep for %i",seconds);
    preSleep();
    // Commenting this out to test the deepPowerDownTime method
    // ab1805.deepPowerDown(seconds);

    // Instead of using the deepPowerDown method from the AB1805 library that limits it to 255 seconds, we will use the deepPowerDownTime method to put the device to sleep until a speciifc time in the future. 
    // Care must be taken to ensure the time is set correctly on the AB1805 before calling this method and that the time is always in the future. Otherwise, you risk going to sleep and never waking up. 
    if (!ab1805.isRTCSet()) {
      ab1805.setRtcFromTime((time_t)1, 0);
    }
    ab1805.getRtcAsTime(time_cv,hundrths_cv);
    ab1805.deepPowerDownTime((uint32_t)time_cv + seconds, hundrths_cv);
    postSleep();
    Log.infoln("Third Test - Failure - Did not power off");
  }

  

  Log.infoln("*********************************************");
}

void loop() {
  ab1805.loop();
}

void wakeUp_Timer() {
    IRQ_Reason = IRQ_AB1805;                          // Just so we know why we woke
}

void preSleep() {
  delay(500);             // Let serial message get out
  LED.off();              // Turn off the LED
}

void postSleep() {
  uint32_t startTime = millis();
  delay(100);
  while(!Serial.availableForWrite()) {
    if ((millis() - startTime) >= 2000) break;
  }
  LED.on();               // Turn on the LED
}