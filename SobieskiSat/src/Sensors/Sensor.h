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
		
		virtual bool begin() = 0;
		virtual bool update() = 0;
		float readValue();
		bool setUpdateDelay(int ms);
		//void loadToSDBuffer(std::initializer_list<float> inputs);
		void (*sendLog)(String message, Sensor& sender);
		
		protected:
		
		long lastUpdate;
		int updateDelay;
		int minDelay;
	};
};

#endif