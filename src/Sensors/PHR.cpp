#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
//#include <cmath>
#include "../src/config.h"


using namespace SobieskiSat;

PHR::PHR() : Sensor() { ID = 'M'; }
		
bool PHR::begin()
{
	fileName = "PHR.txt";
	minDelay = 0;
	setUpdateDelay(UPD_PHR);
	pinMode(PIN_PHR, INPUT);
	
	Initialized = true;
	return true;
}
		
bool PHR::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		Light = analogRead(PIN_PHR);
		
		SDbuffer += String(Light);
		lastUpdate = millis();
		return true;
	}
	else return false;
}
		
String PHR::listReadings()
{
	return "LightIntensity: " + String(Light, 0);
}