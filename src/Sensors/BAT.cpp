#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
//#include <cmath>
#include "../src/config.h"


using namespace SobieskiSat;

BAT::BAT() : Sensor() { ID = 'b'; }
		
bool BAT::begin()
{
	Status = STA_DURINGINIT;
	fileName = "BAT.txt";
	minDelay = 0;
	setUpdateDelay(UPD_BAT);
	pinMode(PIN_BAT, INPUT);
	
	Status = STA_INITIALIZED;
	return (Status == STA_INITIALIZED);
}
		
bool BAT::update()
{
	if (timeForUpdate())
	{
		Reading = analogRead(PIN_BAT);
		
		successUpdateFinish();
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


