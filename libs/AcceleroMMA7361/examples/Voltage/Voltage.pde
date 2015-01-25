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
  x = accelero.getXVolt();
  Serial.print("\nx: ");
  Serial.print(x);
  y = accelero.getYVolt();
  Serial.print("mV\ty: ");
  Serial.print(y);
  z = accelero.getZVolt();
  Serial.print("mV\tz: ");
  Serial.print(z);
  Serial.print("mV");
  delay(100);
}
