#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
#include "../src/config.h"


using namespace SobieskiSat;

MQ9::MQ9() : Sensor() { ID = 'M'; }
		
bool MQ9::begin()
{
	fileName = "MQ9.txt";
	minDelay = 0;
	setUpdateDelay(UPD_MQ9);
	pinMode(PIN_MQ9, INPUT);
	
	// dodać procedurę testu czujnika
	
	Initialized = true;
	return true;
}
		
bool MQ9::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		AirQuality = analogRead(PIN_MQ9);
		
		SDbuffer += String(AirQuality) + "\r\n";
		lastUpdate = millis();
		return true;
	}
	else return false;
}
		
String MQ9::listReadings()
{
	return "AirQuality: " + String(AirQuality, 0);
}