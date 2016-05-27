#include "Bluetooth.h"
#include "Animation.h"
#include "Accelerometer.h"
#include <ShiftRegister74HC595.h>

#define REGISTERS_COUNT 4
#define PIN_LATCH 8 
#define PIN_DATA 11
#define PIN_CLOCK 12

ShiftRegister74HC595 sr(REGISTERS_COUNT, PIN_DATA, PIN_CLOCK, PIN_LATCH);
Animations animation(sr);
Bluetooth bluetooth;

void setup()
{
  pinMode(13, OUTPUT); // Arduino's embedded LED
  Serial.begin(9600);    // or 115200 
  
  bluetooth.init(); // Initialize Bluetooth module

  noInterrupts();           // disable all interrupts
  // initialize timer1 (16bit) in CTC mode — see http://www.locoduino.org/spip.php?article89
  TCCR1A = 0;
  TCCR1B = 0b00001100; // tweak divider, currently 256
  TIMSK1 = 0b00000010;
  TCNT1 = 0;
  OCR1A = 31250; // tweak time

  // Pin change interrupt - doesn't work with SoftwareSerial
  //PCICR = 0b00000100;          // Enable PCINT1 interrupt
  //PCMSK2 = bit (digitalPinToPCMSKbit(BTRX)); // enable only for BTRX pin — see http://playground.arduino.cc/Main/PinChangeInterrupt
  interrupts();             // enable all interrupts
}

// Interrupt for bluetooth update and accelerometer sampling
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(13, digitalRead(13) ^ 1);   // toggle LED pin

  if(updateAccelerometer()) {
    animation.forceAnimById(5, 1, 6);
  }

  if(bluetooth.update()) // update bluetooth
    animation.forceAnimById(bluetooth.getAnim(), bluetooth.getAnimSpeed(), bluetooth.getDuration());
}

void loop()
{
  animation.runNext();
}
