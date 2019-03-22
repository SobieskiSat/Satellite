#include <SobieskiSat.h>
#include <SPI.h>
#include <SD.h>

using namespace SobieskiSat;


Radio radio(10, 12, 433.0, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);

Logger logger;
GPS gps;
BMP280 bmp;
SPS30 sps;
DHT22 dht;

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
long aliveTimer;
bool outsideCan = false;
int packetNo = 0;
long photoTimer;

Compressor compressor;
Player buzzer;
int photo = A2;
String photoBuffer;

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
    ///logger.addToBuffer("Everything is working");
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

  if(millis() - photoTimer >= 250)
    {
    photoTimer = millis();
    photoBuffer += String(analogRead(photo));
    }
  
  if (!catchedEmpty)
  {
    gps.update();
    bmp.update();
    sps.update();
    dht.update();
  }

  if (gps.Initialized && !catchedGPS)
  {
    catchedGPS = true;
  }


  
  if (radio.tx_fifo_empty() && !catchedEmpty)
  {
      lastEmpty = millis();
      catchedEmpty = true;
  }
  
  if (radio.tx_fifo_empty() && millis() - lastEmpty > 40 && catchedEmpty)
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
      compressor.attach(DataPacket("PackNo", 0, 32, 2, 0, packetNo);

      radio.transmit(compressor.data);
      packetNo++;
      if (packetNo >= 32) packetNo = 0;
                        
      digitalWrite(13, state);
      state = !state;

      catchedEmpty = false;
      delay(20);
  }

  if (millis() - lastSave > 2500 && !catchedEmpty)
  {
    delay(10);
    SerialUSB.println("SD saving");
    
    logger.addToBuffer(photoBuffer);
    photoBuffer = "";
    logger.save(gps);
    logger.save(bmp);
    logger.save(sps);
    logger.save(dht);
    logger.saveBuffer();
    lastSave = millis();
    delay(10);
  }

  if (millis() - aliveTimer > 2000 && !doBeep)
  {
    aliveTimer = millis();
    tone(5, 540, 50);
  }

  if (gps.Initialized && gps.Altitude > maxHeight && gps.Altitude > 0 && gps.Altitude < 1000 && !doBeep)
  {
    maxHeight = gps.Altitude;
  }
  if ((millis() > 1000 * 60 * 4 || ( maxHeight - gps.Altitude > 30)) && !doBeep)
  {
    doBeep = true;
    //logger.addToBuffer("Detected landing");
  }

  if (doBeep && millis() - beepTimer > 1000)
  {
    beepTimer = millis();
    tone(5, 440, 750);
  }
}
