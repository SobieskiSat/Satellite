#ifndef SOBIESKISATLIBRARY_COMPRESSOR_H
#define SOBIESKISATLIBRARY_COMPRESSOR_H

#include "Arduino.h"
#include "DataPacket.h"

namespace SobieskiSat
{
	class Compressor
	{
		public:
		
		void begin(int mode);
		void clear();
		void attach(String name, float value);
		void push(int length, char *data_);
		void download(String name, float& variable);
		DataPacket retrieve(String name);
		void generateFormat(String particularNameChain);
		String getData();
		
		bool Transmitter;
		
		private:
		
		char data[256];			// received data is stored here
		int currentBit = 0;		// index of last bit in data
		int receivedLenght = 0;
		String currentFormat = "";
		String formats = "";
		String nameChain = "SNU LAT LON ALT PRE TEM AIR PM10 PM25 PM40 PM100 HUM BATy";
		
		DataPacket getDataPacketLocation(String name, int &startBit, int &endBit);
		void matchFormat();
		
	};
}

#endif