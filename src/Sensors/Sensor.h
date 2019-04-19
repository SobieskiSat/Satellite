#ifndef SOBIESKISATLIBRARY_SENSOR_H
#define SOBIESKISATLIBRARY_SENSOR_H

#include "Arduino.h"
#include "../src/config.h"

namespace SobieskiSat
{
	class Sensor
	{
		public:
		
		int Status = 0;
		String SDbuffer;
		String fileName;
		char ID;
		
		bool begin();
		bool update();
		bool setUpdateDelay(int ms);
		void SDbufferClear();
		String listReadings();
		//static bool (*sendLog)(String message, Sensor& sender);
		
		protected:
		
		long lastUpdate;
		int updateDelay;
		int minDelay;
		bool timeForUpdate();
	};
};

#endif