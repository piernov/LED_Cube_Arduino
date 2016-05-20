#include "Bluetooth.h"
#include "Animation.h"
#include <ShiftRegister74HC595.h>

#define REGISTERS_COUNT 4
#define PIN_LATCH 8 
#define PIN_DATA 11
#define PIN_CLOCK 12

ShiftRegister74HC595 sr(REGISTERS_COUNT, PIN_DATA, PIN_CLOCK, PIN_LATCH);
Animations animation(sr);

void setup()
{
  pinMode(13, OUTPUT); // Arduino's embedded LED
  Serial.begin(9600);    // or 115200 
  
  setupBluetooth(); // Initialize Bluetooth module

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

  animation.add(Anim(&Animation::inorder, 4.0, 6));
  animation.add(Anim(&Animation::random, 4.0, 60));
  animation.add(Anim(&Animation::rain, 4.0, 60));
  animation.add(Anim(&Animation::serpentine, 4.0, 6));
}

// Interrupt for bluetooth update and accelerometer sampling
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  digitalWrite(13, digitalRead(13) ^ 1);   // toggle LED pin

  int a = getAnimBluetooth(); // update bluetooth
  if(a < 0) return;
  animation.setBreak(true); // stop currently running animation
  animation.clear(); // remove current list of animation

  switch(a) { // add animation corresponding to the received number
    case 1:
          animation.add(Anim(&Animation::inorder, 4.0, 6));
          break;
    case 2:
          animation.add(Anim(&Animation::random, 4.0, 60));
          break;
    case 3: 
          animation.add(Anim(&Animation::rain, 4.0, 60));
          break;
    case 4:
          animation.add(Anim(&Animation::serpentine, 4.0, 6));
          break;
  }
}

void loop()
{
  animation.runNext();
}
