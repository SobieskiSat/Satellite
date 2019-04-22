#ifndef SOBIESKISATLIBRARY_COMPRESSOR_H
#define SOBIESKISATLIBRARY_COMPRESSOR_H

#include "Arduino.h"
#include "DataPacket.h"
#include "../src/config.h"

namespace SobieskiSat
{
	class Compressor
	{
		public:
		
		void clear();
		void attach(char name, float value);
		DataPacket retrieve(char name);
		void download(char name, float& variable);
		void push(int length, char* data_);
		String getData();
		
		char data[COMPR_BUFFSIZE];
		int currentBit = 0;
		
		private:
		
		DataPacket find(char name, int &startBit, int &endBit);
		DataPacket getDataPacket(char name);
		int receivedLenght = 0;
		
	};
}

#endif