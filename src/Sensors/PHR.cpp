#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
//#include <cmath>
#include "../src/config.h"


using namespace SobieskiSat;

PHR::PHR() : Sensor() { ID = 'M'; }
		
bool PHR::begin()
{
	Status = STA_DURINGINIT;
	fileName = "PHR.txt";
	minDelay = 0;
	setUpdateDelay(UPD_PHR);
	pinMode(PIN_PHR, INPUT);
	
	// dodać procedurę testu czujnika
	Status = STA_INITIALIZED;
	
	return (Status == STA_INITIALIZED);
}
		
bool PHR::update()
{
	if (timeForUpdate())
	{
		Light = analogRead(PIN_PHR);
		
		successUpdateFinish();
		return true;
	}
	else return false;
}
		
String PHR::listReadings()
{
	return "LightIntensity: " + String(Light, 0);
}