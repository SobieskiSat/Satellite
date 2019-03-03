#ifndef SOBIESKISATLIBRARY_SDLOGGER_H
#define SOBIESKISATLIBRARY_SDLOGGER_H

#include "Arduino.h"
#include "../src/Sensors/Sensor.h"
#include "../src/Structures/Clock.h"
#include "../src/Structures/Time.h"

namespace SobieskiSat
{
	class Logger
	{
		public:
		
		bool Initialized = false;
		bool printUSB = true;
		
		bool begin();
		bool save(Sensor& sensor);
		bool saveBuffer();
		void addToBuffer(String str, bool onlyUSB = false);
		bool logSensor(String message, Sensor& sender);
		
		private:
		
		String rootDir;
		String buffer;
	};
	
	#define SD_pin 11
};

#endif
