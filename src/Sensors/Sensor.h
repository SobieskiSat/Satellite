#ifndef SOBIESKISATLIBRARY_SENSOR_H
#define SOBIESKISATLIBRARY_SENSOR_H

#include "Arduino.h"

namespace SobieskiSat
{
	class Sensor
	{
		public:
		
		bool Initialized = false;
		String SDbuffer;
		String fileName;
		char ID;
		//bool newLog = true;
		//bool packetReady = false;
		
		bool begin();
		bool update();
		bool setUpdateDelay(int ms);
		void SDbufferClear();
		String listReadings();
		//void loadToSDBuffer(std::initializer_list<float> inputs);
		static bool (*sendLog)(String message, Sensor& sender);
		
		protected:
		
		long lastUpdate;
		int updateDelay;
		int minDelay;
		int packetSize;
		int packetCount;
		//bool IsPacketReady();
	};
};

#endif