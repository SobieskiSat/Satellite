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
		void begin(String _formats);
		void clear();
		void attach(DataPacket packet);
		void push(int length, char *_data);
		void download(String name, float& variable);
		DataPacket retrieve(String name);
		
		String format = "empty";
		bool Transmitter;
		
		private:
		
		int packetSize = 16;
		char data[16];
		int index = 0; // index of last bit in data
		
		DataPacket find(String name, int &startBit, int &endBit);
		void generateFormat(int length);
		
	};
}

#endif