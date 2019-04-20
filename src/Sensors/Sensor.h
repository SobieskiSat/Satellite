#ifndef SOBIESKISATLIBRARY_SENSOR_H
#define SOBIESKISATLIBRARY_SENSOR_H

#include "Arduino.h"

namespace SobieskiSat
{
	class Sensor
	{
		public:
		
		int Status;
		String SDbuffer;
		String fileName;
		char ID;
		bool newReading;
		
		Sensor();
		bool begin();
		bool update();
		bool setUpdateDelay(int ms);
		void SDbufferClear();
		String listReadings();
		
		protected:
		
		long lastUpdate;
		int updateDelay;
		int minDelay;
		bool timeForUpdate();
		void successUpdateFinish();
	};
};

#endif