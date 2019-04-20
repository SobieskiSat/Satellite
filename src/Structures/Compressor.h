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
		void begin(String formats_);
		void clear();
		void attach(String name, float value);
		void push(int length, char *data_);
		void download(String name, float& variable);
		DataPacket retrieve(String name);
		String generateFormat(String particularNameChain);
		
		bool Transmitter;
		
		private:
		
		char data[256];			// received data is stored here
		int currentBit = 0;		// index of last bit in data
		int receivedLenght = 0;
		String currentFormat = "empty";
		String formats = "empty";
		String nameChain = "SendNum Latitude Longitude Altitude Pressure Temperature AirQuality PM10 PM25 PM40 PM100 Humidity Battery";
		
		DataPacket getDataPacketLocation(String name, int &startBit, int &endBit);
		void matchFormat();
		
	};
}

#endif