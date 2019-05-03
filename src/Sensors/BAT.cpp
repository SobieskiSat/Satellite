#include "Sensors.h"
#include "Sensor.h"
#include <Wire.h>
#include <stdio.h>
#include <math.h>
#include "Arduino.h"
#include "../src/SobieskiSat.h"

using namespace SobieskiSat;

BAT::BAT() { ID = 'b'; }

bool BAT::begin()
{
	Initialized = false;
	minDelay = 30;
	updateDelay = minDelay;
	fileName = "battery.txt";
	Initialized = true;
	return true;
}

bool BAT::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		Reading = (float)analogRead(A4);
		
		SDbuffer += String(Reading) +" @" + String(millis()) + "\r\n";
		lastUpdate = millis();
		newReading = true;
		return true;
	}
	return false;
}

float BAT::getLevel()
{
	return Reading;
}

String BAT::listReadings()
{
	return "Battery level: " + String(getLevel());
}