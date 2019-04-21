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

  compressor.begin(MODE_TX);
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
  SerialUSB.println("Finished setup!");
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

  if (radio.tx_fifo_empty())
  {
	  delay(30);

      radio.transmit("sending", lastTransmit);
	  SerialUSB.println("sending");
      
      digitalWrite(PIN_LED, state);
      state = !state;
	  delay(30);
  }

  if (millis() - lastSave > 5000)
  {
    delay(30);
	SerialUSB.println("saving");
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
