#include "Bluetooth.h"
#include "Animation.h"
#include <ShiftRegister74HC595.h>

#define REGISTERS_COUNT 3
#define PIN_LATCH 8 
#define PIN_DATA 11
#define PIN_CLOCK 12

ShiftRegister74HC595 sr(REGISTERS_COUNT, PIN_DATA, PIN_CLOCK, PIN_LATCH);
Animations animation(sr);

void setup()
{
//  setupBluetooth();

  Serial.begin(9600);    //115200 si on veut
  Serial.println("Bonjour - Pret pour les commandes AT");

  animation.add(&Animation::inorder, 4, 20);
}



void loop()
{
  animation.runAll();


/*  updateBluetooth();

  animation.inorder(4, 20);
  animation.random(4, 20);
  animation.rain(4, 20);
  animation.serpentine(4, 20);
  

    auto func = [] () { Serial.println("Hello world"); };
    func(); // now call the function

    auto fp = &Animation::serpentine;

    Vector<int> fs;
    fs.push_back((int)&fp);
    fs.push_back(4);
    fs.push_back(20);
    Vector<Vector<int>> v;
    v.push_back(fs);

    auto miaou = v[0];
    auto nyan = miaou[1];
    auto meow = miaou[2];
    Serial.print(((int)*(AnimFunc*)miaou[0]);
    Serial.print(nyan);
    Serial.print(" ");
    Serial.println(meow);

(animation.**((AnimFunc*)miaou[0]))(nyan, meow);
    
    void (Animation::*)(float, int)
    
*/
    
/*
  if(recvChar == 'C') 
  {
    digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
  }
  if(recvChar == 'c')
  {
    digitalWrite(LED, HIGH);    // turn the LED off by making the voltage LO
  }
*/

/*  BTSerie.write("*D");
  BTSerie.print(sonar.ping_cm());
  BTSerie.write("*");
  */
}
