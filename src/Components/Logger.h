#ifndef SOBIESKISATLIBRARY_SDLOGGER_H
#define SOBIESKISATLIBRARY_SDLOGGER_H

#include "Arduino.h"
#include "../src/Sensors/Sensor.h"
#include "../src/Structures/Clock.h"
#include "../src/Structures/Time.h"
#include "../src/config.h"
#include "../src/utilities.h"

namespace SobieskiSat
{
	class Logger
	{
		public:
		
		int Status;
		bool printUSB = true;
		
		bool begin();
		bool save(Sensor& sensor, long& lastSave);
		bool saveBuffer(long& lastSave);
		bool timeForSave(long& lastSave, long& lastTransmit);
		
		private:
		
		String rootDir;
		String buffer;
		static void logMessage(const String msg);
		void pushToBuffer(String str);
		bool isInitialized();
	};
};

#endif
