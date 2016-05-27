#ifndef Animation_h
#define Animation_h

#include <ShiftRegister74HC595.h>

extern int face;

class Animation
{
public:
    Animation(ShiftRegister74HC595 &_sr);
    void allOff();
    void inorder(float aniSpeed, int count);
    void random(float aniSpeed, int count);
    void rain(float aniSpeed, int count);
    void serpentine(float aniSpeed, int count);
    void lightface(float aniSpeed, int count);
    void lightall(float aniSpeed, int count);
    typedef void (Animation::* AnimFunc)(float, int);
protected:
    bool needBreak = false;
    AnimFunc anim[6] = {
      &Animation::inorder,
      &Animation::random,
      &Animation::rain,
      &Animation::serpentine,
      &Animation::lightface,
      &Animation::lightall
    };
private:
    ShiftRegister74HC595 *sr;
    int lednb[3][3][3] = { { {4, 28, 5}, // Should be declared elsewhere, use a macro maybe?
                             {1, 6, 0},
                             {7, 3, 2} },
                           { {11, 25, 13},
                             {12, 8, 15},
                             {10, 9, 14} },
                           { {19, 26, 18},
                             {23, 22, 16},
                             {17, 20, 21} } };

    bool forceBreak();
};

struct Anim { // Stores an animation function with its parameters
  Animation::AnimFunc anim;
  float aniSpeed;
  int count;
  Anim(Animation::AnimFunc anim, float aniSpeed, int count): anim(anim), aniSpeed(aniSpeed), count(count) {};
  Anim() {};
};

class Animations: Animation
{
  public:
      Animations(ShiftRegister74HC595 &_sr);
      void add(Anim a);
      void clear();
      void load();
      void runNext();
      void setBreak(bool v);
      void forceAnimById(int anim, float aniSpeed, int count);
      AnimFunc getAnimById(int n);
  private:
      int curAnim = 0;
      Anim actions[16]; // max 16 chained actions
      int actCount = 0;
};

#endif
