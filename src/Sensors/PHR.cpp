#include "Sensors.h"
#include "Sensor.h"
#include <Wire.h>
#include <stdio.h>
#include <math.h>
#include "Arduino.h"
#include "../src/SobieskiSat.h"

using namespace SobieskiSat;

PHR::PHR() { ID = 'L'; }

bool PHR::begin()
{
	Initialized = false;
	minDelay = 30;
	updateDelay = minDelay;
	fileName = "photo.txt";
	Initialized = true;
	return true;
}

bool PHR::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		Light = (float)analogRead(A1);
		
		SDbuffer += String(Light) + "\r\n";
		lastUpdate = millis();
		return true;
	}
	return false;
}

String PHR::listReadings()
{
	return "Light intensity: " + String(Light);
}