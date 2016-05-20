#include "Animation.h"

Animation::Animation(ShiftRegister74HC595 &_sr) {
  sr = &_sr;
}

void Animations::add(Anim a) {
  actions->push_back(a);
}

void Animations::clear() {
  delete actions;
  actions = new Vector<Anim>;
}

void Animations::setBreak(bool v) {
  needBreak = v;
}

void Animations::runNext() {
  setBreak(false);
  if(curAnim >= actions->size()) curAnim = 0;
  auto fp = (*actions)[curAnim].anim;
  auto arg1 = (*actions)[curAnim].aniSpeed;
  auto arg2 = (*actions)[curAnim].count;
  (this->*fp)(arg1, arg2);
  curAnim++;
}

bool Animation::forceBreak() {
  if(needBreak) sr->setAllHigh();
  return needBreak;
}
void Animation::inorder(float aniSpeed, int count) {
  sr->setAllHigh();
  int d = 1000/aniSpeed;
  for (int i = 0; i < count; i++) {
    for(int z = 0; z < 3; z++) {
      for(int l = 0; l < 3; l++) {
        for(int c = 0; c < 3; c++) {
          if(forceBreak()) return;
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
    if(forceBreak()) return;
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
      if(forceBreak()) return;
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
          if(forceBreak()) return;
          if((l == 1 && c != 2) || (l == 3 && c != 0)) continue;
          int led = lednb[h][l == 3 ? 1 : l][l == 2 ? 2-c : c];
          sr->set(led, LOW);
          delay(d);
        } 
      }
    }
    for(int h = 0; h < 3; h++) { // Center LEDs
      if(forceBreak()) return;
      sr->set(lednb[h][1][1], LOW);
      delay(d);
    }
    sr->setAllHigh();
  }
}
