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
		void generateFormat();
		String getData();
		
		bool Transmitter;
		
		private:
		
		char data[256];			// received data is stored here
		int currentBit = 0;		// index of last bit in data
		int receivedLenght = 0;
		String format = "";
		int formatBitLenght = 0;
		int formatByteLenght = 0;
		int chainElements = 0;
		
		DataPacket getDataPacketLocation(String name, int &startBit, int &endBit);
		//void matchFormat();
		
	};
}

#endif