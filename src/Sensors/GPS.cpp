#include "Sensors.h"
#include "Arduino.h"
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>
#include "../src/Structures/Clock.h"
#include "../src/Structures/Time.h"
#include "../src/SobieskiSat.h"
#include "../src/config.h"

using namespace SobieskiSat;

GPS::GPS() : gps(&Serial1) { ID = 'G'; }

bool GPS::begin()
{
	fileName = "GPS.txt";
	minDelay = 0;
	setUpdateDelay(UPD_GPS);
	
	gps.begin(9600);
	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);  
	gps.sendCommand(PGCMD_ANTENNA);

	delay(1000);
	
	Serial1.println(PMTK_Q_RELEASE);
	return true;
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
			
			SDbuffer += String(Latitude, 7) + " " + String(Longitude, 7) + " " + String(Altitude, 7) + " " + RecievedDate.getString() + " @" + String(millis());
			SDbuffer += "\r\n";
			
			return true;
		}
	}
	
	if (!Initialized && gps.fix /*&& gpsAnyZero()*/)
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

String GPS::listReadings()
{
	return "Latitude: " + String(Latitude, 7) + " Longitude: " + String(Longitude, 7) + " Altitude: " + String(Altitude, 2);
}

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