#include "Arduino.h"
#include "Sensors/Sensors.h"
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>
#include "Structures/Clock.h"
#include "Structures/Time.h"
#include "Manager.h"

using namespace SobieskiSat;

Manager::Manager() : radio(10, 12, 433.0, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8) { }

void Manager::begin()
{
	bmp.begin();
	bmp.setUpdateDelay(45);
	dht.begin();
	dht.setUpdateDelay(2100);
	gps.begin();
	radio.begin();
}

void Manager::update()
{
	if(bmp.update())
	{
		SerialUSB.println(String(bmp.Temperature, 2) + spl + String(bmp.Pressure, 2) + spl + String(gps.Latitude, 4) + spl + String(gps.Longitude, 4) + spl + String(gps.Altitude, 1));
	}
	gps.update();
	
	if (radio.empty())
	{
		 radio.transmit(String(bmp.Temperature, 2) + spl + String(bmp.Pressure, 2) + spl + String(gps.Latitude, 4) + spl + String(gps.Longitude, 4) + spl + String(gps.Altitude, 1));
	}
}