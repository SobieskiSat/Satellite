#include "Arduino.h"
#include "../src/Sensors/Sensor.h"
#include "../src/Structures/Clock.h"
#include "../src/Structures/Time.h"
#include "Logger.h"
#include <SPI.h>
#include <SD.h>
#include "../src/config.h"

using namespace SobieskiSat;

String temp_buffer = "";

bool Logger::begin()
{
	Status = STA_DURINGINIT;
	LogMessage = logMessage;
	
	if (SD.begin(PIN_SD))
	{
		File sessions = SD.open("SESSIONS.TXT");
		if (!sessions)
		{
			sessions = SD.open("SESSIONS.TXT", FILE_WRITE);
			if (sessions)
			{
				sessions.println("0");
				sessions.close();
				SD.mkdir("0");
				rootDir = "0/";
				
				Status = STA_INITIALIZED;
				return true;
			}
			else return false;
		}
		
		int sessionNo = sessions.readStringUntil('\n').toInt() + 1;
		sessions.close();
		SD.remove("SESSIONS.TXT");
		sessions = SD.open("SESSIONS.TXT", FILE_WRITE);
		if (sessions)
		{
			sessions.println(String(sessionNo));
			sessions.close();
			SD.mkdir(String(sessionNo));
			rootDir = String(sessionNo) + "/";
			
			Status = STA_INITIALIZED;
			return true;
		}
		
		return false;
	}
	else return false;
}

bool Logger::save(Sensor& sensor)
{
	if (Status == STA_INITIALIZED)
	{
	if (sensor.SDbuffer != "")
	{
		File file = SD.open(rootDir + sensor.fileName, FILE_WRITE);
		if (file)
		{
			file.print(sensor.SDbuffer);
			sensor.SDbufferClear();
			file.close();
			return true;
		}
		return false;
	}
	}
	return false;
}

void Logger::logMessage(const String msg)
{
	if (LOG_SD == 1) SerialUSB.println(msg);
	if (LOG_SERIAL == 1) temp_buffer += msg + "\r\n";
}

bool Logger::saveBuffer()
{
	buffer += temp_buffer;
	temp_buffer = "";
	if (buffer != "")
	{
		File file = SD.open(rootDir + "LOG.TXT", FILE_WRITE);
		if (file)
		{
			file.print(buffer);
			file.close();
			buffer = "";
			SerialUSB.println("[L] Logged to SD.");
			return true;
		}
	}
	return false;
}