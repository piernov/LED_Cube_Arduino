# LED_Cube_Arduino

Depends on:
	— ShiftRegister74HC595 library: https://github.com/Simsso/Shift-Register-74HC595-Arduino-Library.git
	— Arduino_Vector library: https://github.com/zacsketches/Arduino_Vector.git, and needs the following patch applied:

diff --git a/Vector.h b/Vector.h
index e65fd4e..7a253f0 100644
--- a/Vector.h
+++ b/Vector.h
@@ -32,7 +32,7 @@
 #ifndef VECTOR_H
 #define VECTOR_H JUN_2014
 
-#include <arduino.h>
+#include <Arduino.h>
 
 //as far as I can tell placement new is not included with AVR or arduino.h
 template<typename T>
@@ -70,7 +70,7 @@ class Vector {
        T* elem;
        int space;
        
-       Vector(const Vector&);                  //private copy constrution because I
+//     Vector(const Vector&);                  //private copy constrution because I
                                                                        //have not got this working yet and don't
                                                                        //want to expose this for clients who might
                                                                        //be expecting it.

