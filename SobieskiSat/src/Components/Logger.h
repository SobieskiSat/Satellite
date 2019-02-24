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
		
		String buffer;
		
		bool begin();
		bool save(Sensor& sensor);
		bool saveBuffer();
		//bool logSensor(String message, Sensor& sender);
		
		private:
		
		String rootDir;
	};
	
	#define SD_pin 11
};

#endif
