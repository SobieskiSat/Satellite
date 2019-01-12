#ifndef SOBIESKISATLIBRARY_MANAGER_H
#define SOBIESKISATLIBRARY_MANAGER_H

#include "Arduino.h"
#include "Sensors/Sensors.h"
#include "Components/CanSatKitRadio.h"
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>

namespace SobieskiSat
{
	class Manager
	{
		public:
		
		Manager();
		
		Radio radio;
		BMP280 bmp;
		DHT22 dht;
		GPS gps;
		
		void begin();
		void update();
		
		private:
		
		char spl = '_'; 
	};
};

#endif