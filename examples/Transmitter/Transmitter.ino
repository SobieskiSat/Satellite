#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;

Radio radio(10, 12, 433.8, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);

float sendNum = 0;  // numer wysłanego pakietu
GPS gps;            
BMP280 bmp;
SPS30 sps;
MQ9 mq9;            // (analog)
DHT22 dht;
PHR photo;
BAT battery;

bool state = false;
long lastSave;
long lastTransmit;

Compressor compressor;
Logger logger;
DataPacket current('I', 0, 0, 0, 0);

void setup() {
  SerialUSB.begin(115200);
  pinMode(PIN_LED, OUTPUT);

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
  
  delay(1000);

  lastSave = millis() + 10;
  lastTransmit = millis();
}

void loop() {
  
  gps.update();
  bmp.update();
  sps.update();
  dht.update();
  mq9.update();
  battery.update();
  photo.update();

  if (radio.timeForTransmit(lastSave, lastTransmit))
  {
      compressor.clear();

      current = DAT_SNU;
      current.value = sendNum;
      compressor.attach(current);
      current = DAT_LAT;
      current.value = gps.Latitude;
      compressor.attach(current);
      current = DAT_LAT;
      current.value = gps.Latitude;
      compressor.attach(current);
      current = DAT_LON;
      current.value = gps.Longitude;
      compressor.attach(current);
      current = DAT_ALT;
      current.value = gps.Altitude;
      compressor.attach(current);
      current = DAT_PRE;
      current.value = bmp.Pressure;
      compressor.attach(current);
      current = DAT_TEM;
      current.value = bmp.Temperature;
      compressor.attach(current);
      current = DAT_AIR;
      current.value = mq9.AirQuality;
      compressor.attach(current);
      current = DAT_PM25;
      current.value = sps.PM2_5;
      compressor.attach(current);
      current = DAT_PM100;
      current.value = sps.PM10_0;
      compressor.attach(current);
      current = DAT_HUM;
      current.value = dht.Humidity;
      compressor.attach(current);
      current = DAT_BAT;
      current.value = battery.getLevel();
      compressor.attach(current);

      Frame frame;
      frame.print(compressor.data);
      radio.transmit(frame);
      
      digitalWrite(13, state);
      state = !state;
      lastTransmit = millis();
  }

  if (logger.timeForSave(lastSave, lastTransmit))
  {
    logger.save(gps);
    logger.save(bmp);
    logger.save(mq9);
    logger.save(sps);
    logger.save(dht);
    logger.save(battery);
    logger.save(photo);
    logger.saveBuffer();
    lastSave = millis();
  }
}
