#include <SobieskiSat.h>

using namespace SobieskiSat;

BMP280 sensor; // possible: BMP280, DHT22, SPS30, GPS, MPU

void setup()
{
  SerialUSB.begin(115200); 
  if (sensor.begin()) SerialUSB.println("Initialization complete.");
  else SerialUSB.println("Failed to initialize."); 
}

void loop()
{
  if (sensor.update())
  {
	  SerialUSB.println(sensor.listReadings());
  }
}