#include "Sensors.h"
#include "Arduino.h"
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>
#include "../src/Structures/Clock.h"
#include "../src/Structures/Time.h"

using namespace SobieskiSat;

GPS::GPS() : gps(&Serial1) { ID = 'G'; }

bool GPS::begin()
{
	gps.begin(9600);
	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
	gps.sendCommand(PGCMD_ANTENNA);

	delay(1000);
	
	Serial1.println(PMTK_Q_RELEASE); 
	
	minDelay = 0;
	updateDelay = 0;
	fileName = "GPS.txt";
}

bool GPS::update()
{
	
	char c = gps.read();
	if (gps.newNMEAreceived())
	{
		if (!gps.parse(gps.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
			return false;  // we can fail to parse a sentence in which case we should just wait for another
		else if (gps.fix)
		{
			Latitude = gps.latitudeDegrees;
			Longitude = gps.longitudeDegrees;
			Altitude = gps.altitude;
			LastNMEA = String(gps.lastNMEA());
			HardwareClock.update();
			updateRecievedDate();
			
			return true;
		}
	}
	
	if (!Initialized && gps.fix && gpsAnyZero())
	{
		HardwareClock.begin(RecievedDate);
		updateRecievedDate();
		
		Initialized = true;
		
		Latitude = gps.latitudeDegrees;
		Longitude = gps.longitudeDegrees;
		Altitude = gps.altitude;
		LastNMEA = gps.lastNMEA();
		return true;
	}
	return false;
}

float GPS::readValue() { return 0; }

bool GPS::gpsAnyZero()
{
	return (gps.latitudeDegrees < 1 ? false : true) &&
		   (gps.longitudeDegrees < 1 ? false : true) &&
		   (gps.altitude < 1 ? false : true);
}

void GPS::updateRecievedDate()
{
	RecievedDate.Year = gps.year;
	RecievedDate.Month = gps.month;
	RecievedDate.Day = gps.day;
	RecievedDate.Hour = gps.hour;
	RecievedDate.Minutes = gps.minute;
	RecievedDate.Seconds = gps.seconds;
	RecievedDate.Milliseconds = gps.milliseconds;
}