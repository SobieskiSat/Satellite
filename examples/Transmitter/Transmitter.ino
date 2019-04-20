#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;

Radio radio(10, 12, 433.8, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);
Compressor compressor;
Logger logger;

float sendNum = 0;  // numer wysłanego pakietu
GPS gps;            // obiekty fantomów sensorów przechowujące ostatio otrzymane dane
BMP280 bmp;
SPS30 sps;
MQ9 mq9;            // (analog)
DHT22 dht;
PHT photo;
BAT battery;

Player buzzer;

void setup() {
  SerialUSB.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  buzzer.begin(5);

  compressor.begin();
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

  buzzer.play(2);
  
  lastSave = millis();
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

  if (radio.timeForTransmit())
  {
      compressor.clear();

      compressor.attach("SendNum", sendNum);
      compressor.attach("Latitude", gps.Latitude);
      compressor.attach("Longitude",  gps.Longitude);
      compressor.attach("Altitude", gps.Altitude);
      compressor.attach("Pressure", bmp.Pressure);
      compressor.attach("Temperature", bmp.Temperature);
      compressor.attach("AirQuality", mq9.AirQuality);
      compressor.attach("PM10", sps.PM1_0);
      compressor.attach("PM25", sps.PM2_5);
      compressor.attach("PM40", sps.PM4_0);
      compressor.attach("PM100", sps.PM10_0);
      compressor.attach("Humidity", dht.Humidity);
      compressor.attach("Battery", battery.Level);

      radio.transmit(compressor.getData());
      
      digitalWrite(PIN_LED, state);
      state = !state;

      catchedEmpty = false;
  }

  if (logger.timeForSave())
  {
    logger.save(gps);
    logger.save(bmp);
    logger.save(mq9);
    logger.save(sps);
    logger.save(dht);
    logger.save(battery);
    logger.save(photo);
    logger.saveBuffer();
  }
}
