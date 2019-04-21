#include <SobieskiSat.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "logo.h"

using namespace SobieskiSat;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Radio radio(10, 12, 433.8, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);
Compressor compressor;
Logger logger;

float sendNum = 0;  // numer wysłanego pakietu
float reciNum = 0;  // numer otrzymanego pakietu
float rssi = 0;     // rssi ostatniej transmisji
GPS gps;          // obiekty fantomów sensorów przechowujące ostatio otrzymane dane
BMP280 bmp;
SPS30 sps;
BAT battery;
MQ9 mq9;          // (analog)
DHT22 dht;

bool led_state = true;  // wskaźnik otrzymanego pakietu

void setup()
{
  SerialUSB.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    SerialUSB.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
   
  display.clearDisplay();
  display.display();
  display.clearDisplay();
  delay(50);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  
  DrawLogo();
  display.display();
  display.invertDisplay(true);
  delay(250);
  display.invertDisplay(false);
  delay(500);

  radio.begin();
  compressor.begin(MODE_RX);
}

void loop()
{
  // przyjmowanie pakietu
  char data[256];
  int lenght;
  radio.receive(data, lenght);
  reciNum++;

  // sygnalizacja otrzymanego pakietu
  digitalWrite(PIN_LED, led_state);
  led_state = !led_state;

  compressor.push((int)lenght, data);

  UploadToPhantoms();
  PrintSensors();

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  
  display.print("RSSI: ");
  display.println(rssi);
  display.print("H: ");
  display.println(String(compressor.retrieve("ALT").value));
  display.print("P: ");
  display.println(String(int(compressor.retrieve("SNU").value))+ " " + String(reciNum, 0));
  display.print("T: ");
  display.println(String(compressor.retrieve("TEM").value));
  
  display.display();
  
  compressor.clear();
}


void DrawLogo(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  
  display.display();
  delay(250);
}

// wgrywa odbrane dane do fantomów sensorów
void UploadToPhantoms()
{
  rssi = radio.get_rssi_last();
  compressor.download("SNU", sendNum);
  compressor.download("LAT", gps.Latitude);
  compressor.download("LON", gps.Longitude);
  compressor.download("ALT", gps.Altitude);
  compressor.download("PRE", bmp.Pressure);
  compressor.download("TEM", bmp.Temperature);
  compressor.download("AIR", mq9.AirQuality);
  compressor.download("PM10", sps.PM1_0);
  compressor.download("PM25", sps.PM2_5);
  compressor.download("PM40", sps.PM4_0);
  compressor.download("PM100", sps.PM10_0);
  compressor.download("HUM", dht.Humidity);
  compressor.download("BAT", battery.Reading);
}


// wypisuje dane wszystkich sensorów
void PrintSensors()
{
  SerialUSB.print(String(rssi, 0) + "_");
  SerialUSB.print(String(sendNum, 0) + "_");
  SerialUSB.print(String(gps.Latitude, PREC_LAT) + "_");
  SerialUSB.print(String(gps.Longitude, PREC_LON) + "_");
  SerialUSB.print(String(gps.Altitude, PREC_ALT) + "_");
  SerialUSB.print(String(bmp.Pressure, PREC_PRE) + "_");
  SerialUSB.print(String(bmp.Temperature, PREC_TEM) + "_");
  SerialUSB.print(String(mq9.AirQuality, 0) + "_");
  SerialUSB.print(String(sps.PM1_0, PREC_SPS) + "_");
  SerialUSB.print(String(sps.PM2_5, PREC_SPS) + "_");
  SerialUSB.print(String(sps.PM4_0, PREC_SPS) + "_");
  SerialUSB.print(String(sps.PM10_0, PREC_SPS) + "_");
  SerialUSB.print(String(dht.Humidity, PREC_HUM) + "_");
  SerialUSB.print(String(battery.getLevel(), 0) + "_");
  SerialUSB.println();
}
