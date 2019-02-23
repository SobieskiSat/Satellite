#include <SobieskiSat.h>

using namespace SobieskiSat;

GPS gps;
int prec = 7;

void setup() {
  SerialUSB.begin(115200);

  gps.begin();
}

void loop() {
  if (gps.update())
  {
    SerialUSB.print("Latitude: ");
    SerialUSB.print(String(gps.Latitude, prec));
    SerialUSB.print(" Longitude: ");
    SerialUSB.print(String(gps.Longitude, prec));
    SerialUSB.print(" Altitude: ");
    SerialUSB.println(String(gps.Altitude, prec));
  }
}
