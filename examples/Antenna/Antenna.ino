#include <SobieskiSat.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "logo.h"
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

using namespace SobieskiSat;

// set radio receiver parameters - see comments below
// remember to set the same radio parameters in
// transmitter and receiver boards!
Radio radio(10, 12,
            433.0,                  // frequency in MHz
            Bandwidth_125000_Hz,    // bandwidth - check with CanSat regulations to set allowed value
            SpreadingFactor_9,      // see provided presentations to determine which setting is the best
            CodingRate_4_8);        // see provided presentations to determine which setting is the best
Compressor compressor;
bool led_state = true;

int package_num=0; //zmienna przechowująca liczbę pakietów odebranych (!)

int rssi;
float sendNum;
float gpsLatitude;
float gpsLongitude;
float gpsAltitude;
float bmpPressure;
float bmpTemperature;
float mq9AirQuality;
float spsPM2_5;
float spsPM10_0;
float dhtHumidity;
float batteryReading;

void setup() {
  SerialUSB.begin(115200);
  delay(2000);
  
  pinMode(13, OUTPUT);

if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64 ZMIANA NA 0x3D !!!!!!
    SerialUSB.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever            //PĘTLA FOREVER
  }  
  // start radio module  
  radio.begin();
  
  compressor.clear();
  //compressor.format = "0_26_L_49.0000000_54.5000000_2_7 26_53_Longitude_14.0699997_24.0900002_2_7 53_67_Altitude_0.0_1000.0_4_1 67_90_Pressure_600.0000_1200.0000_4_4 90_103_Temperature_-20.00_50.00_2_2 103_113_PM25_0.0_100.0_3_1 113_123_PM100_0.0_100.0_3_1 X";

//OLED
  display.clearDisplay();
  display.display();
  display.clearDisplay();
  delay(50);
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  //  display.println("SobieskiSat");      //piszę od punktu 0,0

  
  DrawLogo();
  display.display();
  display.invertDisplay(true);
    delay(250);
  display.invertDisplay(false);
    delay(5000);  
}

void loop() {
  // prepare empty space for received frame
  // maximum length is maximum frame length + null termination
  // 255 + 1 byte = 256 bytes
  char data[256];

  // receive data and save it to string
  radio.receive(data);

  digitalWrite(13, led_state);
  led_state = !led_state;

  compressor.clear();
  // get and print signal level (rssi) 
  //SERIAL PRINT
  for (int i = 0; i < 16; i++)
  {
    compressor.data[i] = data[i];
  }

  rssi = radio.get_rssi_last();
  compressor.download('S', sendNum);
  compressor.download('L', gpsLatitude);
  compressor.download('l', gpsLongitude);
  compressor.download('A', gpsAltitude);
  compressor.download('P', bmpPressure);
  compressor.download('T', bmpTemperature);
  compressor.download('Q', mq9AirQuality);
  compressor.download('2', spsPM2_5);
  compressor.download('9', spsPM10_0);
  compressor.download('H', dhtHumidity);
  compressor.download('B', batteryReading);

  int rssi = radio.get_rssi_last();
  SerialUSB.print(String(rssi, 0) + "_");
  SerialUSB.print(String(sendNum, 0) + "_");
  SerialUSB.print(String(gpsLatitude, PREC_LAT) + "_");
  SerialUSB.print(String(gpsLongitude, PREC_LON) + "_");
  SerialUSB.print(String(gpsAltitude, PREC_ALT) + "_");
  SerialUSB.print(String(bmpPressure, PREC_PRE) + "_");
  SerialUSB.print(String(bmpTemperature, PREC_TEM) + "_");
  SerialUSB.print(String(mq9AirQuality, 0) + "_");
  SerialUSB.print(String(spsPM2_5, PREC_SPS) + "_");
  SerialUSB.print(String(spsPM10_0, PREC_SPS) + "_");
  SerialUSB.print(String(dhtHumidity, PREC_HUM) + "_");
  SerialUSB.print(String(batteryReading, 0) + "_");
  SerialUSB.println();
  
  //SerialUSB.println(String(compressor.data));
  //OLED PRINT  

  display.clearDisplay();
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setCursor(0, 0);
  display.print("RSSI: ");
  display.println(rssi);
  
  display.print("H: ");
  display.println(String(gpsAltitude));

  display.setTextSize(1);             // Normal 1:1 pixel scale

  display.print("Packs: ");
  display.println(String(package_num));
  
  display.display();
  
  compressor.clear();
  package_num++;
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
