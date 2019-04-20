#include <SobieskiSat.h>

Compressor compressor;
String formats = "";

void setup() {
  // put your setup code here, to run once:
  compressor.begin();
  SerialUSB.begin();

  SerialUSB.println("Gemerowanie formatów dla stacji odbiorczej: ");
  SerialUSB.print("[");
  formats += compressor.generateFormat("SendNum Latitude Longitude Altitude Pressure Temperature AirQuality PM10 PM25 PM40 PM100 Humidity Battery") + "|";
  SerialUSB.print("##");
  formats += compressor.generateFormat("SendNum Latitude Longitude Altitude Pressure Temperature AirQuality PM10 PM25 PM40 PM100") + "|";
  SerialUSB.print("##");
  formats += compressor.generateFormat("SendNum Latitude Longitude Altitude Pressure Temperature AirQuality") + "|";
  SerialUSB.print("##");
  formats += compressor.generateFormat("SendNum Latitude Longitude Altitude Pressure Temperature");
  SerialUSB.println("##] Zakończono");
  SerialUSB.println("Umieść te wartości w pliku 'compressor_formats.h' znajdującego się folderze z programem stacji naziemnej");
  SerialUSB.println("compressorFormat:");
  SerialUSB.println(formats);
  SerialUSB.println("nameChain:");
  SerialUSB.println("SendNum Latitude Longitude Altitude Pressure Temperature AirQuality PM10 PM25 PM40 PM100 Humidity Battery");
}
