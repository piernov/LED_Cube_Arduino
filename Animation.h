#ifndef Animation_h
#define Animation_h

#include <ShiftRegister74HC595.h>
#include <Vector.h>

class Animation
{
public:
    Animation(ShiftRegister74HC595 &_sr);
    void inorder(float aniSpeed, int count);
    void random(float aniSpeed, int count);
    void rain(float aniSpeed, int count);
    void serpentine(float aniSpeed, int count);
private:
    Vector<Vector<int>> actions;
    ShiftRegister74HC595 *sr;
    int lednb[3][3][3] = { { {4, 25, 5}, // Should be declared elsewhere, use a macro maybe?
                             {1, 6, 0},
                             {7, 3, 2} },
                           { {11, 25, 13},
                             {12, 8, 15},
                             {10, 9, 14} },
                           { {19, 25, 18},
                             {23, 22, 16},
                             {17, 20, 21} } };
};

class Animations: Animation
{
  typedef decltype(&Animation::random) AnimFunc;
  public:
      Animations(ShiftRegister74HC595 &_sr): Animation(_sr) {};
      void add(AnimFunc fp, int arg1, int arg2);
      void runAll();
  private:
      Vector<Vector<int>> actions;
};

#endif
