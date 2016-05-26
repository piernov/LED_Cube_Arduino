#ifndef Bluetooth_h
#define Bluetooth_h

#include <SoftwareSerial.h>  //Software Serial Port

#define BTRX 6    //Pin 10 pour RX (pin0=serial) vert
#define BTTX 7    //Pin 11 pour TX, on peut changer noir
#define BTNAME "AT+NAMEMiaou"
#define BTPIN "AT+PIN1234"

class Bluetooth
{
public:
    Bluetooth();
    void init();
    bool update();
    int getAnim();
    float getAnimSpeed();
    int getDuration();
private:
    SoftwareSerial ss;
    float animSpeed;
    int duration;
    int anim;
};

#endif
