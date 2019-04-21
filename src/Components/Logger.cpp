#include "Arduino.h"
#include "../src/Sensors/Sensor.h"
#include "../src/Structures/Clock.h"
#include "../src/Structures/Time.h"
#include "Logger.h"
#include <SPI.h>
#include <SD.h>
#include "../src/config.h"
#include "../src/utilities.h"

using namespace SobieskiSat;

String temp_buffer = "";

bool Logger::begin()
{
	Status = STA_DURINGINIT;
	//LogMessage = logMessage;
	
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

bool Logger::save(Sensor& sensor, long& lastSave)
{
	if (Status == STA_INITIALIZED)
	{
		if (sensor.SDbuffer != "")
		{
			delay(DEL_BEFSAVE);
			File file = SD.open(rootDir + sensor.fileName, FILE_WRITE);
			if (file)
			{
				file.print(sensor.SDbuffer);
				sensor.SDbufferClear();
				file.close();
				lastSave = millis();
				delay(DEL_AFTSAVE);
				return true;
			}
			delay(DEL_AFTSAVE);
			return false;
		}
	}
	return false;
}

void Logger::logMessage(const String msg)
{
	if (LOG_SD == 1) SerialUSB.println(msg);
	if (LOG_SERIAL == 1)
	{
		temp_buffer += msg;
		temp_buffer += "\r\n";
	}
}

bool Logger::saveBuffer(long& lastSave)
{
	if (Status == STA_INITIALIZED)
	{
		buffer += temp_buffer;
		if (buffer != "")
		{
			delay(DEL_BEFSAVE);
			File file = SD.open(rootDir + "LOG.TXT", FILE_WRITE);
			if (file)
			{
				file.print(buffer);
				file.close();
				buffer = "";
				temp_buffer = "";
				delay(DEL_AFTSAVE);
				return true;
			}
			lastSave = millis();
			delay(DEL_AFTSAVE);
			return false;
		}
	}
	return false;
}

bool Logger::timeForSave(long& lastSave, long& lastTransmit)
{
	if (millis() - lastSave > DEL_BETWEENSAVE &&
		abs(lastSave - lastTransmit) > DEL_SAVETRAN) return true;
}