#include <SobieskiSat.h>

using namespace SobieskiSat;

BMP280 bmp;
int prec = 7;

void setup() {
  SerialUSB.begin(115200);

  bmp.begin();
}

void loop() {
  if (bmp.update())
  {
    SerialUSB.print("Pressure: ");
    SerialUSB.print(String(bmp.Pressure, prec));
    SerialUSB.print(" Temperature: ");
    SerialUSB.print(String(bmp.Temperature, prec));
    SerialUSB.print(" Altitude: ");
    SerialUSB.println(String(bmp.Altitude, prec));
  }
}
