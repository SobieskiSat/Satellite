#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;

//Radio radio(10, 12, 433.0, Bandwidth_500000_Hz, SpreadingFactor_7, CodingRate_4_8);
Radio radio(10, 12, 433.0, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);

Logger logger;
GPS gps;
BMP280 bmp;
SPS30 sps;
DHT22 dht;
MQ9 mq;

long lastSave;
long lastEmpty;
bool catchedEmpty = false;
bool state = true;
bool doGenerate = true;
bool catchedGPS = false;
float maxHeight = -1;
bool isStatic = false;
bool doBeep = false;
long beepTimer;
int lastGPS[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int compareIndex = 0;
long gpsTimer;

Compressor compressor;
Player buzzer;

void setup() {
  SerialUSB.begin(115200);

  pinMode(13, OUTPUT);

  buzzer.begin(5);

  compressor.clear();

  logger.begin();
  
  bmp.begin();
  gps.begin();
  sps.begin();
  dht.begin();

  if (radio.begin() && logger.Initialized && bmp.Initialized && sps.Initialized && dht.Initialized)
  {
    buzzer.play(0);
    logger.addToBuffer("Everything is working");
  }
  else
  {
    buzzer.play(1);
  }
  
  delay(1000);

  buzzer.play(2);
  lastSave = millis();
}

void loop() {
  
  gps.update();
  bmp.update();
  sps.update();
  dht.update();

  if (gps.Initialized && !catchedGPS)
  {
    catchedGPS = true;
    logger.addToBuffer("Recieved real time: " + gps.RecievedDate.getString());
  }

  buzzer.update();
  
  if (radio.empty() && !catchedEmpty)
  {
      lastEmpty = millis();
      catchedEmpty = true;
  }
  
  if (radio.empty() && millis() - lastEmpty > 30 && catchedEmpty)
  { 
      SerialUSB.println("Radio sending");

      compressor.clear();
      
      compressor.attach(DataPacket("Latitude", 49, 54.5, 2, 7, gps.Latitude));
      compressor.attach(DataPacket("Longitude",  14.07, 24.09, 2, 7, gps.Longitude));
      compressor.attach(DataPacket("Altitude", 0, 1000, 4, 1, gps.Altitude));
      compressor.attach(DataPacket("Pressure", 600, 1200, 4, 4, bmp.Pressure));
      compressor.attach(DataPacket("Temperature", -20, 50, 2, 2, bmp.Temperature));
      compressor.attach(DataPacket("PM25", 0, 100, 3, 1, sps.PM2_5));
      compressor.attach(DataPacket("PM100", 0, 100, 3, 1, sps.PM10_0));

      Frame frame;
      frame.print(compressor.data);
      radio.transmit(frame);
      
      digitalWrite(13, state);
      state = !state;

      catchedEmpty = false;
  }

  if (millis() - lastSave > 5000)
  {
    delay(10);
    SerialUSB.println("SD saving");
    logger.save(gps);
    logger.save(bmp);
    logger.save(sps);
    logger.save(dht);
    logger.saveBuffer();
    lastSave = millis();
    delay(10);

    tone(5, 540, 40);
  }

  if (gps.Initialized = true && millis() - gpsTimer > 1000 && !doBeep)
  {
    if (compareIndex < 10)
    {
      lastGPS[compareIndex] = (int)gps.Altitude;
      compareIndex++;
    }
    else
    {
      for (int i = 1; i < 10; i++)
      {
        lastGPS[i] = lastGPS[i - 1];
      }
      lastGPS[0] = (int)gps.Altitude;

      int differences = 0;
      for (int i = 1; i < 10; i++)
      {
        differences += lastGPS[i] - lastGPS[i - 1];
      }
      if (abs(differences) < 1 && !isStatic)
      {
        isStatic = true;
        logger.addToBuffer("Can is static");
      }
      else if (isStatic)
      {
        logger.addToBuffer("Can is not static");
        isStatic = false;
      }

    }
    gpsTimer = millis();
  }

  if (gps.Initialized = true && gps.Altitude > maxHeight && gps.Altitude > 0 && gps.Altitude < 1000 && !doBeep)
  {
    maxHeight = gps.Altitude;
  }
  if (millis() > 1000 * 60 * 5 && maxHeight - gps.Altitude > 30 && isStatic && !doBeep)
  {
    doBeep = true;
    logger.addToBuffer("Detected landing");
  }

  if (doBeep && millis() - beepTimer > 1000)
  {
    beepTimer = millis();
    tone(5, 600, 750);
  }
}