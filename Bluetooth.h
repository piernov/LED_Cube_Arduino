#include <SoftwareSerial.h>  //Software Serial Port
#include <Arduino.h>

#define BTRX 6    //Pin 10 pour RX (pin0=serial) vert
#define BTTX 7    //Pin 11 pour TX, on peut changer noir
#define BTNAME "AT+NAMEMiaou"
#define BTPIN "AT+PIN1234"

void setupBluetooth();

void updateBluetooth();
