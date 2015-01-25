#include <AcceleroMMA7361.h>

AcceleroMMA7361 accelero(13, 12, 11, A0, A1, A2, 5, -40, 20);
double x;
double y;
double z;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  x = accelero.getXRaw();
  Serial.print("\nx: ");
  Serial.print(x);
  y = accelero.getYRaw();
  Serial.print("\ty: ");
  Serial.print(y);
  z = accelero.getZRaw();
  Serial.print("\tz: ");
  Serial.print(z);
  delay(100);
}
