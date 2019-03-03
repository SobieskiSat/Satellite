#ifndef SOBIESKISATLIBRARY_DATAPACKET_H
#define SOBIESKISATLIBRARY_DATAPACKET_H

#include "Arduino.h"
#include <math.h>

namespace SobieskiSat
{
	class DataPacket
	{
		// data starts from the most significant digit (MSD)
		public:
		
		DataPacket(String name_, float bottom_limit, float upper_limit, int numbers_, int decimals_, float value_)
		{
			name = name_;
			bottomLimit = bottom_limit;
			upperLimit = upper_limit;
			numbers = numbers_;
			decimals = decimals_;
			value = value_;
		};
		
		String name;
		float bottomLimit;
		float upperLimit;
		int numbers;
		int decimals;
		float value;
	};
}

#endif