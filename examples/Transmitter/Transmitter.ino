#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;

Radio radio(10, 12, 433.0, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);
Logger logger;
GPS gps;
BMP280 bmp;
MPU mpu;
SPS30 sps;
DHT22 dht;

int buzzer = 5;

bool state = true;
long lastBlink;
char spl = '_';

void setup() {
  SerialUSB.begin(115200);

  pinMode(13, OUTPUT);
  pinMode(buzzer, OUTPUT);

  if (!logger.begin())
  { 
    SerialUSB.println("Card failed, or not present");
  }
  
  gps.begin();
  logger.buffer += "Intitialization of: " + String(bmp.ID) + " > " + (bmp.begin() == true ? "OK" : "UNABLE") + " @" + millis() + "\r\n";
  logger.buffer += "Intitialization of: " + String(mpu.ID) + " > " + (mpu.begin() == true ? "OK" : "UNABLE") + " @" + millis() + "\r\n"; 
  logger.buffer += "Intitialization of: " + String(sps.ID) + " > " + (sps.begin() == true ? "OK" : "UNABLE") + " @" + millis() + "\r\n"; 
  logger.buffer += "Intitialization of: " + String(dht.ID) + " > " + (dht.begin() == true ? "OK" : "UNABLE") + " @" + millis() + "\r\n"; 

  radio.begin();
  
  lastBlink = millis();
}

void loop() {
  gps.update();
  bmp.update();
  mpu.update();
  sps.update();
  dht.update();

  if (radio.empty())
  {
    SerialUSB.println("Sending via radio.");
    radio.transmit(String(bmp.Temperature, 2) + spl + String(bmp.Pressure, 2) + spl + String(gps.Latitude, 7) + spl + String(gps.Longitude, 7) + spl + String(gps.Altitude, 1));
    SerialUSB.println("Saving to SD");
    logger.save(gps);
    logger.save(bmp);
    logger.save(sps);
    logger.save(mpu);
    logger.save(dht);
    logger.saveBuffer();
  }

  if (millis() - lastBlink >= 250)
  {
    lastBlink = millis();
    digitalWrite(13, state);
    state = !state;
  }
}
