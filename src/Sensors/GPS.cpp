#include "Sensors.h"
#include "Arduino.h"
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>
#include "../src/Structures/Clock.h"
#include "../src/Structures/Time.h"
#include "../src/config.h"

using namespace SobieskiSat;

GPS::GPS() : gps(&Serial1) { ID = 'G'; }

bool GPS::begin()
{
	Status = STA_DURINGINIT;
	fileName = "GPS.txt";
	minDelay = 0;
	setUpdateDelay(UPD_GPS);
	
	gps.begin(9600);
	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);  
	gps.sendCommand(PGCMD_ANTENNA);

	delay(1000);
	
	Serial1.println(PMTK_Q_RELEASE);
	
	// dodać procedurę testu i kalibracji
	
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
			
			SDbuffer += LastNMEA;
			SDbuffer += "\r\n";
			SDbuffer += String(Latitude, PREC_LAT) + " " + String(Longitude, PREC_LON) + " " + String(Altitude, PREC_ALT) + " " + RecievedDate.getString() + " @" + String(millis());
			SDbuffer += "\r\n";
						
			return true;
		}
	}
	
	if ((Status == STA_DURINGINIT) && gps.fix)
	{
		HardwareClock.begin(RecievedDate);
		updateRecievedDate();
		
		Latitude = gps.latitudeDegrees;
		Longitude = gps.longitudeDegrees;
		Altitude = gps.altitude;
		LastNMEA = gps.lastNMEA();
		
		// procedura testu i kalibracji - gpsAnyZero
		Status = STA_INITIALIZED;
		
		return true;
	}
	return false;
}

String GPS::listReadings()
{
	return "Latitude: " + String(Latitude, PREC_LAT) + " Longitude: " + String(Longitude, PREC_LON) + " Altitude: " + String(Altitude, PREC_ALT);
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