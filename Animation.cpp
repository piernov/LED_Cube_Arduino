#include "Animation.h"

Animation::Animation(ShiftRegister74HC595 &_sr) {
  sr = &_sr;
}

void Animations::add(Anim a) {
  actions.push_back(a);
}

void Animations::runAll() {
  for(int i=0; i < actions.size(); i++) {
    auto fp = actions[i].anim;
    auto arg1 = actions[i].aniSpeed;
    auto arg2 = actions[i].count;
    (this->*fp)(arg1, arg2);
  }
}

void Animation::inorder(float aniSpeed, int count) {
  sr->setAllHigh();
  int d = 1000/aniSpeed;
  for (int i = 0; i < count; i++) {
    for(int z = 0; z < 3; z++) {
      for(int l = 0; l < 3; l++) {
        for(int c = 0; c < 3; c++) {
          sr->set(lednb[z][l][c], LOW);
          delay(d);
        }
      }
    }
    sr->setAllHigh();
  }
}

void Animation::random(float aniSpeed, int count) {
  sr->setAllHigh();
  int d = 1000/aniSpeed;
  for(int i = 0; i < count; i++) {
    int h = ::random(0,3);
    int l = ::random(0,3);
    int c = ::random(0,3);
    int led = lednb[h][l][c];
    sr->set(led, 1-sr->get(led));
    delay(d);
  }
  sr->setAllHigh();
}

void Animation::rain(float aniSpeed, int count) {
  sr->setAllHigh();
  int d = 1000/aniSpeed;
  for(int i = 0; i < count; i++) {
    int l = ::random(0,3);
    int c = ::random(0,3);
    for(int h = 2; h >= 0; h--) {
      int led = lednb[h][l][c];
      sr->set(led, LOW);
      delay(d);
      sr->set(led, HIGH);
    }
    delay(d);
  }
  sr->setAllHigh();
}

void Animation::serpentine(float aniSpeed, int count) {
  sr->setAllHigh();
  int d = 1000/aniSpeed;
  for(int i = 0; i < count; i++) {
    for(int h = 2; h >= 0; h--) {
      for(int l = 0; l < 4; l++) {
        for(int c = 0; c < 3; c++) {
          if((l == 1 && c != 2) || (l == 3 && c != 0)) continue;
          int led = lednb[h][l == 3 ? 1 : l][l == 2 ? 2-c : c];
          sr->set(led, LOW);
          delay(d);
        } 
      }
    }
    for(int h = 0; h < 3; h++) { // Center LEDs
      sr->set(lednb[h][1][1], LOW);
      delay(d);
    }
    sr->setAllHigh();
  }
}
