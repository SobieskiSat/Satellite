#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;

Radio radio(10, 12, 433.8, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);
Compressor compressor;
Logger logger;

float sendNum = 0;  // numer wysłanego pakietu
GPS gps;            
BMP280 bmp;
SPS30 sps;
MQ9 mq9;            // (analog)
DHT22 dht;
PHR photo;
BAT battery;

Player buzzer;
bool state = false;
long lastSave;
long lastTransmit;

void setup() {
  SerialUSB.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  buzzer.begin(5);

  compressor.clear();
  logger.begin();

  gps.begin();
  bmp.begin();
  sps.begin();
  dht.begin();
  mq9.begin();
  battery.begin();
  photo.begin();
  
  radio.begin();

  lastSave = millis() + 10;
  lastTransmit = millis();
  
  buzzer.play(2);
}

void loop() {

  gps.update();
  bmp.update();
  sps.update();
  dht.update();
  mq9.update();
  battery.update();
  photo.update();

  buzzer.update();

  if (radio.timeForTransmit(lastSave, lastTransmit))
  {
	  delay(30);
      compressor.clear();

      compressor.attach('S', sendNum);
      compressor.attach('L', gps.Latitude);
      compressor.attach('l',  gps.Longitude);
      compressor.attach('A', gps.Altitude);
      compressor.attach('P', bmp.Pressure);
      compressor.attach('T', bmp.Temperature);
      compressor.attach('Q', mq9.AirQuality);
      compressor.attach('1', sps.PM1_0);
      compressor.attach('2', sps.PM2_5);
      compressor.attach('4', sps.PM4_0);
      compressor.attach('9', sps.PM10_0);
      compressor.attach('H', dht.Humidity);
      compressor.attach('B', battery.Level);

      radio.transmit(compressor.getData(), lastTransmit);
      
      digitalWrite(PIN_LED, state);
      state = !state;
	  delay(30);
  }

  if (logger.timeForSave(lastSave, lastTransmit))
  {
    delay(30);
    logger.save(gps, lastSave);
    logger.save(bmp, lastSave);
    logger.save(mq9, lastSave);
    logger.save(sps, lastSave);
    logger.save(dht, lastSave);
    logger.save(battery, lastSave);
    logger.save(photo, lastSave);
    logger.saveBuffer(lastSave);
	delay(30);
  }
}
