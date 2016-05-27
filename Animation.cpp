#include "Animation.h"

Animation::Animation(ShiftRegister74HC595 &_sr) {
  sr = &_sr;
}

Animation::AnimFunc Animations::getAnimById(int n) {
  if(n < 5) return anim[n];
  else return anim[0];
}

Animations::Animations(ShiftRegister74HC595 &_sr): Animation(_sr) {
  this->load();
};

void Animations::add(Anim a) {
  actions[actCount] = a;
  actCount++;
}

void Animations::clear() {
  curAnim = 0;
  actCount = 0;
}

void Animations::load() {
  this->add(Anim(&Animation::inorder, 4.0, 15));
  this->add(Anim(&Animation::random, 4.0, 15));
  this->add(Anim(&Animation::rain, 4.0, 15));
  this->add(Anim(&Animation::serpentine, 4.0, 15));
}

void Animations::setBreak(bool v) {
  needBreak = v;
}

void Animations::runNext() {
  if(curAnim >= actCount) {
    curAnim = 0;
    return;
  }
  bool hasLooped = false;
  setBreak(false);

  auto fp = actions[curAnim].anim;
  auto arg1 = actions[curAnim].aniSpeed;
  auto arg2 = actions[curAnim].count;

  (this->*fp)(arg1, arg2);
  if(needBreak) {
    curAnim = 0;
    return;
  }
  curAnim++;
  if(curAnim >= actCount) {
    if(arg2 == -1)
      curAnim--;
    else {
      if(actCount == 4) // Assume default if there are 4 anims programmed
        curAnim = 0;
      else {
        this->clear();
        this->load();
      }
    }
  }
}

void Animations::forceAnimById(int anim, float aniSpeed, int duration) {
  if(anim <= 0 || aniSpeed <= 0 || duration == 0 || duration < -1) return; // Don't trust the user...
  this->setBreak(true); // stop currently running animation
  this->clear(); // remove current list of animation
  if(anim == 38) // Reset to default animations
    this->load();
  else
    this->add(Anim(this->getAnimById(anim-1), aniSpeed, duration));
}

bool Animation::forceBreak() {
  if(needBreak) sr->setAllHigh();
  return needBreak;
}

void debugAnim(String name, float aniSpeed, int count) {
  Serial.print("Animation: ");
  Serial.print(name);
  Serial.print(", duration: ");
  Serial.print(count);
  Serial.print(", speed: ");
  Serial.println(aniSpeed);
}

void Animation::inorder(float aniSpeed, int count) {
  int d = 1000/aniSpeed;
  if(count > 0)
    count = 1 + (count*1000L / (d*27L)); // 27 LED with d ms between each
  else count = 1;
  debugAnim("inorder", aniSpeed, count);
  sr->setAllHigh();
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
  int d = 1000/aniSpeed;
  if(count > 0)
    count = 1 + (count*1000L / d); // d ms between each LED
  else count = 1;
  debugAnim("random", aniSpeed, count);
  sr->setAllHigh();
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
  int d = 1000/aniSpeed;
  if(count > 0)
    count = 1 + (count*1000L / (d*4L)); // d ms between each column of 3 LED + d ms between each of the 3 LED
  else count = 1;
  debugAnim("rain", aniSpeed, count);
  sr->setAllHigh();
  for(int i = 0; i < count; i++) {
    int l = ::random(0,3); // random from global namespace, not the above
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
  int d = 1000/aniSpeed;
  if(count > 0)
    count = 1 + (count*1000L / (d*27L)); // 27 LED with d ms between each;
  else count = 1;
  debugAnim("serpentine", aniSpeed, count);
  sr->setAllHigh();
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

int faces[6][9] = { {4, 28, 5, 1, 6, 0, 7, 3, 2},
                    {19, 26, 18, 23, 22, 16, 17, 20, 21},
                    {4, 1, 7, 11, 12, 10, 19, 23, 17},
                    {5, 0, 2, 13, 15, 14, 18, 16, 21},
                    {4, 28, 5, 11, 25, 13, 19, 26, 18},
                    {7, 3, 2, 10, 9, 14, 17, 20, 21} };

void Animation::lightface(float aniSpeed, int count) {
  if(face < 1 || face > 6) return;
  debugAnim("serpentine", face, count);
  sr->setAllHigh();
  for(int i = 0; i < 9; i++)
  {
    sr->set(faces[face-1][i], LOW);
  }
  for(int i = 0; i < count; i++) { // just wait
    if(forceBreak()) return;
    delay(1000);
  }
  sr->setAllHigh();
}


