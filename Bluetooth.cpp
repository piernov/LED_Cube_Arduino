#include "Bluetooth.h"

SoftwareSerial BTSerie(BTRX, BTTX);

//void callback() {
//  Serial.println("Bluetooth Callback");
//  updateBluetooth();
//}

void setupBluetooth() {
    // Configuration du bluetooth
  pinMode(BTRX, INPUT);
  pinMode(BTTX, OUTPUT);
  
  BTSerie.begin(9600);  //57600
//  BTSerie.setInterruptCallback(&callback);
  delay(500);
  BTSerie.print("AT");  // Starts communication
  delay(500);
  BTSerie.print(BTNAME); // Set bluetooth module name
  Serial.println(BTNAME);
  delay(500);
  BTSerie.print(BTPIN); // Set bluetooth module PIN code
  Serial.println(BTPIN);
  delay(500);
}

// The following needs to be modified to handle selecting the animation along with its parameters
int getAnimBluetooth() {
  if(!BTSerie.available()) return -1;
  char c = BTSerie.read();
  if(c == 'A') {
    String miaou =  BTSerie.readStringUntil('p');
    return miaou.toInt();
  }
  else if(c == 'S') {
    // add animation speed parameter handling here
  }
  else if(c == 'D') {
    // add animation duration parameter handling here
  }
  return -1;
}

