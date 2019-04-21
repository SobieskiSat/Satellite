#include "DataPacket.h"
#include "Arduino.h"
#include "../src/config.h"

using namespace SobieskiSat;

DataPacket::DataPacket (String name_, float bottom_limit, float upper_limit, int decimals_)
{
	name = name_;
	bottomLimit = bottom_limit;
	upperLimit = upper_limit;
	decimals = decimals_;
	/*
	if (value > upperLimit)
	{
		SerialUSB.println(name + " is above range, assigned overflow value.");
		value = upperLimit - value_ + bottomLimit;
		//state = State.overflow;
	}
	else if (value < bottomLimit)
	{
		SerialUSB.println(name + " is below range, assigned overflow value.");
		value = upperLimit - (bottomLimit - value);
		//state = State.overflow;
	}
	//else state = State.normal;
	*/
}

String DataPacket::toString()
{
	if (decimals > 0)
	return  name + "_" + 
			String(bottomLimit, decimals) + "_" +
			String(upperLimit, decimals) + "_" +
			String(decimals);
	else
	return  name + "_" + 
			String((int)bottomLimit) + "_" +
			String((int)upperLimit) + "_" +
			String(decimals);
}

int DataPacket::getBitCount()
{
	unsigned int spread = (unsigned int)((upperLimit - bottomLimit) * pow(10, decimals));
	int packetBits = 0;
	while (spread > pow(2, packetBits)) { packetBits++; }
	return packetBits;
}