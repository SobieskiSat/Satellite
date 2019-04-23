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
		
		void clear();
		void attach(DataPacket packet);
		DataPacket retrieve(char name);
		
		int packetSize = 24;
		char data[24];
		bool generateFormat = false;
		String format = "";
		// index of last bit in data
		int index = 0;
		
		void download(char name, float& variable);
		private:
		
		DataPacket find(char name, int &startBit, int &endBit);
		
		
	};
}

#endif