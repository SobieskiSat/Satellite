#include "Sensors.h"
#include "Arduino.h"
#include "Sps30/sensirion_uart.h"
#include "Sps30/sps30.h"
#include "../src/SobieskiSat.h"
#include "../src/config.h"

using namespace SobieskiSat;

SPS30::SPS30() { ID = 'S'; }

bool SPS30::begin()
{
	Initialized = false;
	fileName = "SPS30.txt";
	minDelay = 1000;
	setUpdateDelay(UPD_SPS);
	
	sensirion_uart_open();
	for (int i = 0; i < 5; i++)
	{
		if (sps30_probe() == 0 && 
			sps30_start_measurement() == 0)
		{
			lastUpdate = millis() + DEL_SPS;
			Initialized = true;
			return true;
		}
		delay(1000);
	}
	
	sensirion_uart_close();
	return Initialized;
}

bool SPS30::update()
{
	if (millis() - lastUpdate > updateDelay && Initialized)
	{
		ret = sps30_read_measurement(&measurement);
		if (ret < 0) return false;
		
		PM1_0 = measurement.mc_1p0;
		PM2_5 = measurement.mc_2p5;
		PM4_0 = measurement.mc_4p0;
		PM10_0 = measurement.mc_10p0;
		
		SDbuffer += String(PM1_0, 4) + " " + String(PM2_5, 4) + " " + String(PM4_0, 4) + " " + String(PM10_0, 4) + " @" + String(millis());
		SDbuffer += "\r\n";
				
		lastUpdate = millis();
		
		return true;
	}
	return false;
}

String SPS30::listReadings()
{
	return "PM1_0: " + String(PM1_0) + " PM2_5: " + String(PM2_5) + " PM4_0: " + String(PM4_0) + " PM10_0: " + String(PM10_0);
}