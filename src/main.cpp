#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <ArduinoLog.h>
#include <AB1805_RK.h>
#include "pinout.h"

#define IRQ_AB1805 1

AB1805 ab1805(Wire); // Class instance for the the AB1805 RTC

// put function declarations here:
void wakeUp_Timer();

// Variables - we do not have a time source
volatile uint8_t IRQ_Reason = 0; 										// 0 - Invalid, 1 - AB1805, 2 - RFM95 DIO0, 3 - RFM95 IRQ, 4 - User Switch, 5 - Sensor

void setup() {
	Serial.begin(115200);												      // Establish Serial connection if connected for debugging
	delay(2000);                                      // Let the serial connect

  // Log.begin(LOG_LEVEL_SILENT, &Serial);
	Log.begin(LOG_LEVEL_INFO, &Serial);
	Log.infoln("See Insights LoRa Node - Power Off Sleep Test");

  gpio.setup(); 														        // Setup the pins

  ab1805.withFOUT(gpio.WAKE).setup();               // Set up the RTC with WAKE on pin 8

  LowPower.attachInterruptWakeup(gpio.WAKE, wakeUp_Timer, FALLING);   // Attach the interrupt

  { // First Test - Regular Sleep
    Log.infoln("First Test - sleep for 10 seconds");
    LowPower.sleep(10000);                          // Sleep for 10 seconds
    Log.infoln("Awoke from first test - pause for 10 seconds");
  }

  delay (10000);                                     // This simply keeps the serial connection active

  { // Second Test - Wake on interrupt
    Log.infoln("Second Test - sleep for 10 seconds - wake on clock (WAKE pin = %s)", (digitalRead(gpio.WAKE)?"High":"Low"));
    ab1805.interruptCountdownTimer(10,false);        // 10 seconds - no clock set required
    LowPower.sleep(12000);                           // Set the alarm for 12 seconds as a backstop

    if (IRQ_Reason == IRQ_AB1805) {
      Log.infoln("Woke on clock - success (WAKE pin = %s)", (digitalRead(gpio.WAKE)) ? "High":"Low");         // Woke by clock on WAKE pin
    }
    else {
      Log.infoln("Woke on timeout - failure (WAKE pin = %s)", (digitalRead(gpio.WAKE)) ? "High":"Low");       // Woke by the timeout
    }
    IRQ_Reason = 0;
  }


}

void loop() {
  ab1805.loop();
}

void wakeUp_Timer() {
    IRQ_Reason = IRQ_AB1805;                          // Just so we know why we woke
}