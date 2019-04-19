#include "Arduino.h"
#include "Sensor.h"
#include "../src/config.h"

using namespace SobieskiSat;

bool Sensor::setUpdateDelay(int ms) // dodać log
{
	if (ms == 0)
	{
		updateDelay = minDelay;
		// LOG: Minimalna wartość odświeżania ustawiona
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
	//packetReady = false;
}

bool Sensor::timeForUpdate()
{
	return (millis() - lastUpdate > updateDelay && Status == STA_INITIALIZED);
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