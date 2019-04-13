#include <SobieskiSat.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "logo.h"
#include "compressor_formats.h"

using namespace SobieskiSat;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Radio radio(10, 12, 433.0, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);
Compressor compressor;
Logger logger;

int sendNum = 0;  // numer wysłanego pakietu
int reciNum = 0;  // numer otrzymanego pakietu
int rssi = 0;     // rssi ostatniej transmisji
GPS gps;          // obiekty fantomów sensorów przechowujące ostatio otrzymane dane
BMP280 bmp;
SPS30 sps;
int battery = 0;  // poziom baterii (analog)
MQ9 mq9;          // (analog)
DHT22 dht;

#define led_pin 13      // pin leda (wskaźnika)
bool led_state = true;  // wskaźnik otrzymanego pakietu

void setup()
{
  SerialUSB.begin(115200);
  pinMode(led_pin, OUTPUT);

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
  compressor.begin(compessorFormat);
}

void loop()
{
  // przyjmowanie pakietu
  char data[256];
  radio.receive(data);
  reciNum++;

  // sygnalizacja otrzymanego pakietu
  digitalWrite(led_pin, led_state);
  led_state = !led_state;

  compressor.set(data);

  UploadToSensors();
  PrintSensors();

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  
  display.print("RSSI: ");
  display.println(rssi);
  display.print("H: ");
  display.println(String(compressor.retrieve("Altitude").value));
  display.print("P: ");
  display.println(String(int(compressor.retrieve("PackNo").value))+ " " + String(package_num) + " " + String(package_num % 32));
  display.print("T: ");
  display.println(String(compressor.retrieve("Temperature").value));
  
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
void UploadToSensors()
{
  compressor.upload("SendNum", &sendNum);
  rssi = radio.get_rssi_last();
  compressor.upload("Latitude", &gps.Latitude);
  compressor.upload("Longitude", &gps.Longitude);
  compressor.upload("Altitude", &gps.Altitude);
  compressor.upload("Pressure", &bmp.Pressure);
  compressor.upload("Temperature", &bmp.Temperature);
  compressor.upload("AirQuality", &mq9.AirQuality);
  compressor.upload("PM10", &sps.PM1_0);
  compressor.upload("PM25", &sps.PM2_5);
  compressor.upload("PM40", &sps.PM4_0);
  compressor.upload("PM100", &sps.PM10_0);
  compressor.upload("Humidity", &dht.Humidity);
  compressor.upload("Battery", &battery);
}


// wypisuje dane wszystkich sensorów
void PrintSensors()
{
  SerialUSB.print(String(sendNum, 0) + "_");
  SerialUSB.print(String(rssi, 0) + "_");
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
  SerialUSB.print(String(battery, 0) + "_");
  SerialUSB.println();
}
