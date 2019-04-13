#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
#include <cmath>


using namespace SobieskiSat;

MQ9::MQ9()
{
	analogReadResolution(12);
	ID = 'L';
	Initialized = false;
}
		
bool MQ9::begin()
{
	fileName = "MQ9.txt";
	minDelay = 0;
	setUpdateDelay(UPD_MQ9);
	
	float lastReading = 1000;
	int errorCounter = 0;
	for (int i = 0; i < 20; i++)
	{ 
		lastReading = AirQuality;
		AirQuality = analogRead(PIN_MQ9);
		if (i > 2 && (lastReading - AirQuality > 20 || lastReading - AirQuality < -20)) errorCounter++;
		delay(10);
	}
	if (errorCounter > 10)
	{
		Initialized = false;
		return false;
	}
	
	Initialized = true;
	return true;
}
		
bool MQ9::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		AirQuality = analogRead(PIN_MQ9);
		
		lastUpdate = millis();
		return true;
	}
	else return false;
}
		
String BMP280::listReadings()
{
	return "AirQuality: " + String(AirQuality, PREC_AIR);
}