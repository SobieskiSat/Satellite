#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;

//Radio radio(10, 12, 433.0, Bandwidth_500000_Hz, SpreadingFactor_7, CodingRate_4_8);
Radio radio(10, 12, 433.8, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);

Logger logger;
GPS gps;
BMP280 bmp;
SPS30 sps;
DHT22 dht;
BAT battery;
PHR photo;
MQ9 mq9;

Compressor compressor;
//Player buzzer;

long lastSave;
long beepTimer;
bool state;
float sendNum = 0;
bool generated = false;
bool longerPacket = false;

void setup() {
  SerialUSB.begin(115200);

  pinMode(13, OUTPUT);

  //buzzer.begin(5);

  compressor.clear();

  logger.begin();
  
  bmp.begin();
  gps.begin();
  sps.begin();
  dht.begin();
  battery.begin();
  mq9.begin();
  photo.begin();
  
  radio.begin();
  
  delay(1000);

  //buzzer.play(2);
  lastSave = millis();
}

void loop() {
  gps.update();
  if (millis() - lastSave >= 200) bmp.update();
  if (sps.update()) longerPacket = true;
  dht.update();
  battery.update();
  mq9.update();
  photo.update();


  if (radio.tx_fifo_empty())
  { 
      SerialUSB.println("Radio sending");
      if (!generated) {generated = true; compressor.generateFormat = true; }
      compressor.clear();
      compressor.attach(DataPacket("SendNum", 0, 255, 0, 0, sendNum));
      compressor.attach(DataPacket("Latitude", 49, 54.5, 2, 7, gps.Latitude));
      compressor.attach(DataPacket("Longitude",  14.07, 24.09, 2, 7, gps.Longitude));
      compressor.attach(DataPacket("Altitude", 0, 1000, 4, 1, gps.Altitude));
      compressor.attach(DataPacket("Pressure", 600, 1200, 4, 4, bmp.Pressure));
      compressor.attach(DataPacket("Temperature", -20, 50, 2, 2, bmp.Temperature));
      compressor.attach(DataPacket("AirQuality", 0, 1024, 4, 0, mq9.AirQuality));
      if (sps.newReading)
      {
        
      compressor.attach(DataPacket("PM25", 0, 100, 3, 1, sps.PM2_5));
      compressor.attach(DataPacket("PM100", 0, 100, 3, 1, sps.PM10_0));
      
      sps.newReading = false;
      if (dht.newReading)
      {
        compressor.attach(DataPacket("Humidity", 0, 100, 3, 1, dht.Humidity));
        compressor.attach(DataPacket("Battery", 0, 1024, 4, 0, battery.Reading));
        
        dht.newReading = false;
      }
      }
      compressor.generateFormat = false;
      Frame frame;
      frame.clear();
      for (int i = 0; i <= compressor.index / 8; i++)
        {
          frame.print(compressor.data[i]);
        }

      radio.transmit(frame);
      sendNum++;
      digitalWrite(13, state);
      state = !state;
      longerPacket = false;

      gps.newReading = false;
      bmp.newReading = false;
      mq9.newReading = false;
      battery.newReading = false;
      photo.newReading = false;
      mq9.newReading = false;
  }

  if (millis() - lastSave > 5000)
  {
    SerialUSB.println("SD saving");
    logger.save(gps);
    logger.save(bmp);
    logger.save(sps);
    logger.save(dht);
    logger.save(photo);
    logger.save(battery);
    logger.save(mq9);
    logger.saveBuffer();
    lastSave = millis();

    tone(5, 440, 250);
  }
}
