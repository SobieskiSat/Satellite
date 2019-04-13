#include "Arduino.h"
#include "Sensors.h"
#include "Sensor.h"
#include <cmath>


using namespace SobieskiSat;

MQ9::MQ9()
{
	pin = PIN_MQ9;
	analogReadResolution(12);
	ID = 'L';
}
		
bool MQ9::begin()
{
	Initialized = false;
	minDelay = 0;
	fileName = "MQ9.txt";
	
	float lastTemperature = 1000;
	int errorCounter = 0;
	for (int i = 0; i < 50; i++)
	{ 
		lastTemperature = Temperature;
		update();
		if (i > 2 && (lastTemperature - Temperature > 10 || lastTemperature - Temperature < -10))
			errorCounter++;
		delay(1);
	}
	if (errorCounter > 10)
	{
		//sendLog("uin", *this); // uin - unsuccesful initialization
		return false;
	}
			
	//sendLog("sin", *this); // sin - succesful initialization
	Initialized = true;
	return true;
}
		
bool MQ9::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		Temperature = 100.0 * (analogRead(pin) * 3.3 / (std::pow(2, 12)));
		//loadToSDBuffer({Temperature});
		
		lastUpdate = millis();
		return true;
	}
	else return false;
}
		
float MQ9::readValue() { update(); return Temperature; }