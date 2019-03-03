#ifndef SOBIESKISATLIBRARY_CLOCK_H
#define SOBIESKISATLIBRARY_CLOCK_H

#include "Arduino.h"
#include "Time.h"

namespace SobieskiSat
{
	class Clock
	{
		public:
		void begin(String initializer);
		void begin(Time initializer);
		
		Time Start;
		Time Current;
		long Tick;
		
		void update();
		Time TickToDate(long tick);
		Time DateToTick(Time date);
		String DateToString(Time date);
		
		private:
		long lastTick;
	};
};

#endif