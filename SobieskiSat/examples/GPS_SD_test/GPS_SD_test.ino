#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;

GPS gps;
int prec = 7;
String str;

void setup() {
  SerialUSB.begin(115200);

  File file = SD.open("gps.txt", FILE_WRITE);
  if (file) 
  {
    file.println("###############################################");
    file.close();
  }

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
    str += String(gps.Latitude, prec) + " " + String(gps.Longitude, prec) + " " + String(gps.Altitude, prec);
    File file = SD.open("gps.txt", FILE_WRITE);
    if (file) 
    {
      file.println(str);
      file.close();
    }

    str = "";
  }
}
