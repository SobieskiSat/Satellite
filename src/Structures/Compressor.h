#ifndef SOBIESKISATLIBRARY_COMPRESSOR_H
#define SOBIESKISATLIBRARY_COMPRESSOR_H

#include "Arduino.h"
#include "DataPacket.h"
#include "../src/Sensors/Sensor.h"

namespace SobieskiSat
{
	class Compressor
	{
		public:
		
		void begin();
		void clear();
		void attach(DataPacket packet);
		void set(char *_data);
		
		DataPacket retrieve(String name);
		
		int packetSize = 16;
		char data[16];
		bool generateFormat = false;
		String format = "";
		// index of last bit in data
		int index = 0;
		
		private:
		
		DataPacket find(String name, int &startBit, int &endBit);
		
		
	};
}

#endif