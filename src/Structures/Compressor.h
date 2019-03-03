#ifndef SOBIESKISATLIBRARY_COMPRESSOR_H
#define SOBIESKISATLIBRARY_COMPRESSOR_H

#include "Arduino.h"
#include "DataPacket.h"

namespace SobieskiSat
{
	class Compressor
	{
		public:
		
		void clear();
		void attach(DataPacket packet);
		DataPacket retrieve(String name);
		
		int packetSize = 47;
		char data[47];
		bool generateFormat = false;
		String format = "";
		
		private:
		
		// index of last bit in data
		int index = 0;
		
		DataPacket find(String name, int &startBit, int &endBit);
		
		
	};
}

#endif