#include <SobieskiSat.h>

using namespace SobieskiSat;

BMP280 sensor; // possible: BMP280, DHT22, SPS30, GPS, MPU

void setup()
{
  SerialUSB.begin(112500); 
  if (sensor.begin()) SerialUSB.println("Initialization complete.");
  else SerialUSB.println("Failed to initialize. (in case of GPS wait for Fix)"); 
}

void loop()
{
  if (sensor.update())
  {
	  SerialUSB.println(sensor.listReadings());
  }
}