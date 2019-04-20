#ifndef SOBIESKISATLIBRARY_DATAPACKET_H
#define SOBIESKISATLIBRARY_DATAPACKET_H

#include "Arduino.h"
#include <math.h>

namespace SobieskiSat
{
	class DataPacket // change to struct??
	{
		public:
		
		DataPacket(String name_, float bottom_limit, float upper_limit, int decimals_);
		
		String name;
		float bottomLimit;
		float upperLimit;
		int decimals;
		float value;
		
		String toString();
		int getBitCount();
	};
}

#endif