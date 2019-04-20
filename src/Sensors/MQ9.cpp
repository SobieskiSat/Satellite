#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
#include <cmath>
#include "../src/config.h"


using namespace SobieskiSat;

MQ9::MQ9() : Sensor() { ID = 'M'; }
		
bool MQ9::begin()
{
	Status = STA_DURINGINIT;
	fileName = "MQ9.txt";
	minDelay = 0;
	setUpdateDelay(UPD_MQ9);
	pinMode(PIN_MQ9, INPUT);
	
	// dodać procedurę testu czujnika
	Status = STA_INITIALIZED;
	
	return (Status == STA_INITIALIZED);
}
		
bool MQ9::update()
{
	if (timeForUpdate())
	{
		AirQuality = analogRead(PIN_MQ9);
		
		successUpdateFinish();
		return true;
	}
	else return false;
}
		
String MQ9::listReadings()
{
	return "AirQuality: " + String(AirQuality, 0);
}