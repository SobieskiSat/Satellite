#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
//#include <cmath>
#include "../src/config.h"


using namespace SobieskiSat;

BAT::BAT() : Sensor() { ID = 'b'; }
		
bool BAT::begin()
{
	fileName = "BAT.txt";
	minDelay = 0;
	setUpdateDelay(UPD_BAT);
	pinMode(PIN_BAT, INPUT);
	Initialized = true;
	return true;
}
		
bool BAT::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		Reading = analogRead(PIN_BAT);
		
		SDbuffer += String(getLevel());
		lastUpdate = millis();
		return true;
	}
	else return false;
}
		
String BAT::listReadings()
{
	return "BatteryLevel: " + String(getLevel(), 0);
}

float BAT::getLevel()
{
	return Reading;
}


