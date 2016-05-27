// max/min analog values
#define MINAV 257
#define MAXAV 422

// analog input pins
#define X_PIN 0
#define Y_PIN 1
#define Z_PIN 2

int face = 0;

int getFace(int pitch, int roll) {
  if(pitch <= 40 && pitch >= -40 && roll <= 40 && roll >= -40)
    return 1;
  if(pitch <= 40 && pitch >= -40 && roll <= 130 && roll >= 50)
    return 2;
  if(pitch <= 40 && pitch >= -40 && roll <= -50 && roll >= -130)
    return 3;
  if(pitch <= -140 && pitch >= 140 && roll <= 40 && roll >= -40)
    return 4;
  if(pitch <= 130 && pitch >= 50 && roll <= 40 && roll >= -40)
    return 5;
  if(pitch <= -50 && pitch >= -130 && roll <= 40 && roll >= -40)
    return 6;
  else
    return 0;
}

bool updateAccelerometer() {
  // sample the voltages
  int x = analogRead(X_PIN);
  int y = analogRead(Y_PIN);
  int z = analogRead(Z_PIN);

  // convert to range of -90 to +90 degrees
  int xAng = map(x, MINAV, MAXAV, -90, 90);
  int yAng = map(y, MINAV, MAXAV, -90, 90);
  int zAng = map(z, MINAV, MAXAV, -90, 90);

  // convert radians to degrees
  int pitch = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  int roll = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);

  // convert left roll and forward pitch to negative degrees
  if (pitch > 180) {
    pitch = pitch - 360;
  }
  if (roll > 180) {
    roll = roll - 360;
  }
  Serial.println("miaou");
  Serial.println(pitch);
  Serial.println(roll);
  Serial.println(x);
  Serial.println(y);
  Serial.println(z);

  int nface = getFace(pitch, roll);
  if(nface != 0 && face != nface) {
    Serial.println(nface);
    face = nface;
    return true;
  }
  return false;
}

