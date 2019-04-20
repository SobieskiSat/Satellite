#include "Arduino.h"
#include "Sensor.h"
#include "../src/config.h"

using namespace SobieskiSat;

Sensor::Sensor()
{
	Status = STA_PREINIT;
	SDbuffer = "";
	fileName = "unknown.txt";
	ID = 'n';
	newReading = false;
}

bool Sensor::setUpdateDelay(int ms) // dodać log
{
	if (ms == 0)
	{
		updateDelay = minDelay;
		return false;
	}
	else if (minDelay > ms || ms < 0)
	{
		updateDelay = minDelay;
		return false;
	}
	
	updateDelay = ms;
	return true;
}

void Sensor::SDbufferClear()
{
	SDbuffer = "";
}

bool Sensor::timeForUpdate()
{
	return (millis() - lastUpdate > updateDelay && Status == STA_INITIALIZED);
}

void Sensor::successUpdateFinish()
{
	lastUpdate = millis();
	newReading = true;
}

/*
bool Sensor::IsPacketReady()
{
	if (packetCount >= packetSize)
	{
		packetCount = 0;
		packetReady = true;
		return true;
	}
	return false;
}
*/
/*
void Sensor::loadToSDBuffer(std::initializer_list<float> inputs)
{
	for (auto element : inputs)
	{
		SDbuffer += String(element, 4);
		SDbuffer += "\t";
	}
	SDbuffer +="@" + String(millis(), DEC) + "\n";
}
*/