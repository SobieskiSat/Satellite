#include "Sensors.h"
#include "Arduino.h"
#include "../src/Components/Logger.h"
#include "../src/config.h"

//extern Logger logger;

using namespace SobieskiSat;

DHT22::DHT22() { ID = 'D'; }

bool DHT22::begin()
{
	Status = STA_DURINGINIT;
	fileName = "DHT22.txt";
	minDelay = 2000;
	lastUpdate = millis() + DEL_DHT;
	setUpdateDelay(UPD_DHT);
	
	// procedura testu i kalibracji
	Status = STA_INITIALIZED;
	
	return (Status == STA_INITIALIZED);
}

bool DHT22::update()
{
	if (timeForUpdate())
	{
		byte data[40] = {0};
		if (sample(data) != SimpleDHTErrSuccess) return false;

		short temperature = 0;
		short humidity = 0;
		if (parse(data, &temperature, &humidity) != SimpleDHTErrSuccess) return false;

		Temperature = (float)((temperature & 0x8000 ? -1 : 1) * (temperature & 0x7FFF)) / 10.0;
		Humidity = (float)humidity / 10.0;

		SDbuffer += String(Humidity, PREC_HUM) + " " + String(Temperature, PREC_TEM) + " @" + String(millis());
		SDbuffer += "\r\n";
		
		lastUpdate = millis();
		return true;
	}
	
	return false;
}

String DHT22::listReadings()
{
	return "Humidity: " + String(Humidity, PREC_HUM) + " Temperature: " + String(Temperature, PREC_TEM);
}

long DHT22::levelTime(byte level, int firstWait, int interval)
{
    unsigned long time_start = micros();
    long time = 0;

    bool loop = true;
    for (int i = 0 ; loop; i++)
	{
        if (time < 0 || time > levelTimeout)
		{
            return -1;
        }

        if (i == 0)
		{
            if (firstWait > 0)
			{
                delayMicroseconds(firstWait);
            }
        }
		else if (interval > 0)
		{
            delayMicroseconds(interval);
        }

        // for an unsigned int type, the difference have a correct value
        // even if overflow, explanation here:
        //     https://arduino.stackexchange.com/questions/33572/arduino-countdown-without-using-delay
        time = micros() - time_start;

        loop = (digitalRead(PIN_DHT) == level);
	}

    return time;
}

byte DHT22::bits2byte(byte data[8]) {
    byte v = 0;
    for (int i = 0; i < 8; i++)
	{
        v += data[i] << (7 - i);
    }
    return v;
}

int DHT22::parse(byte data[40], short* ptemperature, short* phumidity)
{
    short humidity = bits2byte(data);
    short humidity2 = bits2byte(data + 8);
    short temperature = bits2byte(data + 16);
    short temperature2 = bits2byte(data + 24);
    byte check = bits2byte(data + 32);
    byte expect = (byte)humidity + (byte)humidity2 + (byte)temperature + (byte)temperature2;
    if (check != expect)
	{
        return SimpleDHTErrDataChecksum;
    }

    *ptemperature = temperature<<8 | temperature2;
    *phumidity = humidity<<8 | humidity2;

    return SimpleDHTErrSuccess;
}

int DHT22::sample(byte data[40]) {
    // empty output data.
    memset(data, 0, 40);

    // According to protocol: http://akizukidenshi.com/download/ds/aosong/AM2302.pdf
    // notify DHT11 to start:
    //    1. T(be), PULL LOW 1ms(0.8-20ms).
    //    2. T(go), PULL HIGH 30us(20-200us), use 40us.
    //    3. SET TO INPUT.
    pinMode(PIN_DHT, OUTPUT);
    digitalWrite(PIN_DHT, LOW);
    delayMicroseconds(1000);
    // Pull high and set to input, before wait 40us.
    // @see https://github.com/winlinvip/SimpleDHT/issues/4
    // @see https://github.com/winlinvip/SimpleDHT/pull/5
    digitalWrite(PIN_DHT, HIGH);
    pinMode(PIN_DHT, INPUT);
    delayMicroseconds(40);

    // DHT11 starting:
    //    1. T(rel), PULL LOW 80us(75-85us).
    //    2. T(reh), PULL HIGH 80us(75-85us).
    long t = 0;
    if ((t = levelTime(LOW)) < 30) {
        return simpleDHTCombileError(t, SimpleDHTErrStartLow);
    }
    if ((t = levelTime(HIGH)) < 50) {
        return simpleDHTCombileError(t, SimpleDHTErrStartHigh);
    }

    // DHT11 data transmite:
    //    1. T(LOW), 1bit start, PULL LOW 50us(48-55us).
    //    2. T(H0), PULL HIGH 26us(22-30us), bit(0)
    //    3. T(H1), PULL HIGH 70us(68-75us), bit(1)
    for (int j = 0; j < 40; j++) {
          t = levelTime(LOW);          // 1.
          if (t < 24) {                    // specs says: 50us
              return simpleDHTCombileError(t, SimpleDHTErrDataLow);
          }

          // read a bit
          t = levelTime(HIGH);              // 2.
          if (t < 11) {                     // specs say: 26us
              return simpleDHTCombileError(t, SimpleDHTErrDataRead);
          }
          data[ j ] = (t > 40 ? 1 : 0);     // specs: 22-30us -> 0, 70us -> 1
    }

    // DHT11 EOF:
    //    1. T(en), PULL LOW 50us(45-55us).
    t = levelTime(LOW);
    if (t < 24) {
        return simpleDHTCombileError(t, SimpleDHTErrDataEOF);
    }

    return SimpleDHTErrSuccess;
}
