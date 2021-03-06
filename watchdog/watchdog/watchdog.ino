#include <avr/wdt.h>
#define buttonPin 2

int buttonState;

void WDT_on()
{ wdt_enable(0x40); // Watchdog Interrupt Enable (page 47): 0b01000000
  /* Start time sequence */
//  WDTCSR |= (1 << WDCE) | (1 << WDE);
  
  WDTCSR |= (1 << WDCE) ;
  /* Set new prescaler(time-out) value = 64K cycles (~0.5 s) */
  WDTCSR = (1 << WDE)| (1 << WDP2) | (1 << WDP0); // datasheet Page 48
}

void WDT_off()
{
  wdt_disable();
  wdt_reset();
  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1 << WDRF);
  /* Write logical one to WDCE and WDE */
  /* Keep old prescaler setting to prevent unintentional time-out
  */
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  /* Turn off WDT */
  WDTCSR = 0x00;
}

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(4, HIGH);
}

void loop() {
 buttonState = digitalRead(buttonPin);

  if (buttonState ==  LOW) {
    digitalWrite(4,LOW);
    WDT_on();
    digitalWrite(3,HIGH);
    delay(600);
    WDT_off();
  } 
}
