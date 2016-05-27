#include "Bluetooth.h"
#include "Animation.h"
#include "Accelerometer.h"
#include <ShiftRegister74HC595.h>
#include <NewPing.h>

#define REGISTERS_COUNT 4
#define PIN_LATCH 8 
#define PIN_DATA 11
#define PIN_CLOCK 12

ShiftRegister74HC595 sr(REGISTERS_COUNT, PIN_DATA, PIN_CLOCK, PIN_LATCH);
Animations animation(sr);
Bluetooth bluetooth;
//NewPing sonar(3,4,15); // trigger - echo - max dist

#define PIN_TRIGGER 3
#define PIN_ECHO 4

bool triggered = false;

void setup()
{
  pinMode(13, OUTPUT); // Arduino's embedded LED
  pinMode(PIN_TRIGGER, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
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

  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(100);
  digitalWrite(PIN_TRIGGER, LOW);

//  int pingcm = sonar.ping_cm();
  int pingcm = pulseIn(PIN_ECHO, HIGH, 1600);
  Serial.print("Dist: ");
  Serial.println(pingcm);
  if(pingcm > 0) {
    if( !triggered ) {
       triggered = true;
      animation.forceAnimById(6, 1, 60);
    }
  } else {
    triggered = false;
  }
}

void loop()
{
  animation.runNext();
}
