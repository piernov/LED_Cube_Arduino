#include "Bluetooth.h"

SoftwareSerial BTSerie(BTRX, BTTX);

void setupBluetooth() {
    // Configuration du bluetooth
  pinMode(BTRX, INPUT);
  pinMode(BTTX, OUTPUT);
  BTSerie.begin(9600);  //57600
  delay(500);
  BTSerie.print("AT+VERSION");  //Demande le N° de version
  delay(500);
  BTSerie.print(BTNAME);
  delay(500);
  BTSerie.print(BTPIN);
  delay(500);
}

void updateBluetooth() {
  char recvChar;
  //On lit caractere par caractere sur le BTSerie et on affiche sur le Terminal Serie
  if (BTSerie.available()) {
    recvChar =  BTSerie.read();
    Serial.print(recvChar);
  }
}

