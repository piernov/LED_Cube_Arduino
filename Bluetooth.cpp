#include "Bluetooth.h"
#include <Arduino.h>

Bluetooth::Bluetooth() : ss(BTRX, BTTX) {
  anim = 0;
  animSpeed = 4.0;
  duration = 15;
};

void Bluetooth::init() {
  pinMode(BTRX, INPUT);
  pinMode(BTTX, OUTPUT);

  ss.begin(9600);  //57600
  ss.print("AT");  // Starts communication
  delay(500);
  ss.print(BTNAME); // Set bluetooth module name
  Serial.println(BTNAME);
  delay(500);
  ss.print(BTPIN); // Set bluetooth module PIN code
  Serial.println(BTPIN);
  delay(500);
}

bool Bluetooth::update() {
  if(!ss.available()) return false;
  char c = ss.read();
  if(c == 'A') // Anim number
    anim =  ss.readStringUntil('p').toInt();
  else if(c == 'S') // Anim speed
    animSpeed = ss.readStringUntil('p').toFloat();
  else if(c == 'D') // Anim duration
    duration = ss.readStringUntil('p').toInt();
  else if(c == 'U')
  {
    ss.read();
    return true;
  }
  return false;
}

float Bluetooth::getAnimSpeed() {
  return animSpeed;
}

int Bluetooth::getDuration() {
  return duration;
}

int Bluetooth::getAnim() {
  return anim;
}

