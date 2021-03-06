#include <SobieskiSat.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

using namespace SobieskiSat;

// set radio receiver parameters - see comments below
// remember to set the same radio parameters in
// transmitter and receiver boards!
Radio radio(10, 12,
            433.8,                  // frequency in MHz
            Bandwidth_125000_Hz,    // bandwidth - check with CanSat regulations to set allowed value
            SpreadingFactor_9,      // see provided presentations to determine which setting is the best
            CodingRate_4_8);        // see provided presentations to determine which setting is the best
Logger logger;
Compressor compressor;
bool led_state = true;

int package_num=0; //zmienna przechowująca liczbę pakietów odebranych (!)

float SendNum = 0;
float Latitude = 0;
float Longitude = 0;
float Altitude = 0;
float Pressure = 0;
float Temperature = 0;
float AirQuality = 0;
float PM25 = 0;
float PM100 = 0;
float Humidity = 0;
float Battery = 0;

long lastSave;

int rssis[79];
int startAlt = 166;
int altitudes[79];

//---------------------------------------------OLED_START_LOGO---------------------------
#define LOGO_HEIGHT   64
#define LOGO_WIDTH    64
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B11110000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000001, B11000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00001000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000111, B11100000, B00000010, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B01111111, B11111110, B00000000, B00000000, B00000000, 
  B00000000, B00000010, B00000011, B11111111, B11111111, B11000000, B00000000, B00000000, 
  B00000000, B00000100, B00001111, B11111111, B11111111, B11110000, B00000000, B00000000, 
  B00000000, B00001000, B00011111, B11111111, B11111111, B11111000, B00000000, B00000000, 
  B00000000, B00000000, B01111111, B11111111, B11111111, B11111110, B00000000, B00000000, 
  B00000000, B00000000, B11111111, B11111111, B11111111, B11111111, B00000100, B00000000, 
  B00000000, B00000001, B11111111, B00000000, B00000000, B11111111, B10000010, B00000000, 
  B00000000, B00000011, B11110000, B00000000, B00000000, B00001111, B11000000, B00000000, 
  B00000000, B00000111, B11000000, B00000000, B00000000, B00000011, B11100000, B00000000, 
  B00000000, B00000110, B00000000, B00000000, B00000000, B00000000, B01100000, B00000000, 
  B00000001, B00001100, B00000000, B00111111, B11111100, B00000000, B00110000, B00000000, 
  B00000000, B00010000, B00000111, B11111111, B11111111, B11100000, B00001000, B00000000, 
  B00000010, B00010000, B00011111, B11111111, B11111111, B11111000, B00001000, B00000000, 
  B00000000, B00110000, B01111111, B11111111, B11111111, B11111110, B00001100, B00000000, 
  B00000000, B00110000, B11111111, B11111111, B11111111, B11111111, B00001100, B00000000, 
  B00000000, B00110000, B11100000, B00010000, B00001000, B00000111, B00001100, B00100000, 
  B00000000, B01110000, B11100000, B00010000, B00001000, B00000111, B00001110, B00000000, 
  B00000000, B01110000, B11100111, B11110011, B11001111, B00111111, B00001110, B00000000, 
  B00000000, B01110000, B01100000, B00010000, B00001111, B00111110, B00001110, B00000000, 
  B00000000, B01110000, B01100000, B00010000, B00001111, B00111110, B00001110, B00000000, 
  B00001000, B11111000, B01111111, B10010011, B11001111, B00111110, B00011111, B00000000, 
  B00001000, B11111000, B01100000, B00010011, B11001111, B00111110, B00011111, B00000000, 
  B00001000, B11111000, B00100000, B00010011, B11001111, B00111100, B00011111, B00000000, 
  B00001000, B11111000, B00111111, B11111111, B11111111, B11111100, B00011111, B00000000, 
  B00001000, B11111100, B00111111, B11111111, B11111111, B11111100, B00111111, B00000000, 
  B00000000, B11111100, B00011111, B11111111, B11111111, B11111000, B00111111, B00000000, 
  B00000000, B01111110, B00011111, B11111111, B11111111, B11111000, B01111110, B00010000, 
  B00000000, B01111110, B00001111, B11111110, B01111111, B11110000, B01111110, B00000000, 
  B00000000, B01111111, B00001111, B11111110, B01111111, B11110000, B11111110, B00000000, 
  B00000000, B01111111, B00000111, B11111100, B00111111, B11100000, B11111110, B00100000, 
  B00000000, B00111111, B10000111, B11111000, B00011111, B11100001, B11111100, B00100000, 
  B00000000, B00111111, B10000011, B11111000, B00011111, B11000001, B11111100, B00000000, 
  B00000010, B00111111, B11000001, B11111100, B00111111, B10000011, B11111100, B00000000, 
  B00000010, B00011111, B11000001, B11111110, B01111111, B10000011, B11111000, B00000000, 
  B00000000, B00011111, B11100000, B11111110, B01111111, B00000111, B11111000, B00000000, 
  B00000001, B00001111, B11110000, B01111111, B11111110, B00001111, B11110000, B00000000, 
  B00000000, B00000111, B11110000, B00111111, B11111100, B00001111, B11100000, B00000000, 
  B00000000, B00000111, B11111000, B00011111, B11111000, B00011111, B11100001, B00000000, 
  B00000000, B00000011, B11111100, B00001111, B11110000, B00111111, B11000000, B00000000, 
  B00000000, B00000001, B11111110, B00000111, B11100000, B01111111, B10000010, B00000000, 
  B00000000, B00000000, B11111111, B00000001, B10000000, B11111111, B00000100, B00000000, 
  B00000000, B00000000, B01111111, B10000000, B00000001, B11111110, B00000000, B00000000, 
  B00000000, B00001000, B00011111, B11000000, B00000011, B11111000, B00000000, B00000000, 
  B00000000, B00000100, B00001111, B11110000, B00001111, B11110000, B00000000, B00000000, 
  B00000000, B00000010, B00000011, B11111000, B00011111, B11000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B01111110, B01111110, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000111, B11100000, B00000010, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00001100, B00000000, B00000000, 
  B00000000, B00000000, B00000110, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B11000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B01110000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
  
};

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
  logger.begin();
  compressor.clear();
  compressor.format = "0_8_SendNum_0_255_0_0 8_34_Latitude_49.0000000_54.5000000_2_7 34_61_Longitude_14.0699997_24.0900002_2_7 61_75_Altitude_0.0_1000.0_4_1 75_98_Pressure_600.0000_1200.0000_4_4 98_111_Temperature_-20.00_50.00_2_2 111_121_AirQuality_0_1024_4_0 121_131_PM25_0.0_100.0_3_1 131_141_PM100_0.0_100.0_3_1 141_151_Humidity_0.0_100.0_3_1 151_161_Battery_0_1024_4_0 ";
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
}

void loop() {
  // prepare empty space for received frame
  // maximum length is maximum frame length + null termination
  // 255 + 1 byte = 256 bytes
  char data[123];

  int lenght;
  // receive data and save it to string
  radio.receive(data, lenght);

  digitalWrite(13, led_state);
  led_state = !led_state;
  lenght -= 1;
  compressor.clear();
  // get and print signal level (rssi) 
  //SERIAL PRINT
  for (int i = 0; i < lenght; i++)
  {
    compressor.data[i] = data[i];
  }

  int rssi = radio.get_rssi_last();
  compressor.retrieve("SendNum", lenght, SendNum);
  compressor.retrieve("Latitude", lenght, Latitude);
  compressor.retrieve("Longitude", lenght, Longitude);
  compressor.retrieve("Altitude", lenght, Altitude);
  compressor.retrieve("Pressure", lenght, Pressure);
  compressor.retrieve("Temperature", lenght, Temperature);
  compressor.retrieve("AirQuality", lenght, AirQuality);
  compressor.retrieve("PM25", lenght, PM25);
  compressor.retrieve("PM100", lenght, PM100);
  compressor.retrieve("Humidity", lenght, Humidity);
  lenght += 1;
  compressor.retrieve("Battery", lenght, Battery);
  SerialUSB.println(String(rssi) + "_" + String((int)SendNum) + "_" + String(Latitude, 7) + "_" + String(Longitude, 7) + "_" + String(Altitude, 1) + "_" + String(Pressure, 4) + "_" + String(Temperature - 2.1, 2) + "_" + String((int)AirQuality) + "_" + String(PM25, 1) + "_" + String(PM100, 1) + "_" + String(Humidity, 1) + "_" + String((int)Battery));
                    
  
  //SerialUSB.println(String(compressor.data));
  //OLED PRINT  

  display.clearDisplay();
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setCursor(0, 0);
  //display.print("RSSI: ");
  //display.println(rssi);
  //display.print("H: ");
  //display.println(String(Altitude));
  //display.print("P: ");
  //display.println(String(package_num) + " " + String((int)SendNum));


  arrayPush(rssis, 79, (-(rssi + 30) * 16.0 / 130.0));
  display.drawLine(0, 17, 78, 17, 1);
  for (int i = 0; i < 79; i++)
  {
    //display.drawLine(i, 16, i, rssis[i], 1); // solid graph
    display.drawPixel(i, rssis[i], 1);
  }
  display.setCursor(79, 0);
  display.setTextSize(2);
  if (rssi > -100) display.print(" ");
  display.print(String(rssi));

  arrayPush(altitudes, 79, ((Altitude - startAlt) * 16.0 / 2700.0));
  display.drawLine(0, 34, 78, 34, 1);
  for (int i = 0; i < 79; i++)
  {
    display.drawLine(i, 32, i, 32 - altitudes[i], 1); // solid graph
    //display.drawPixel(i, rssis[i], 1);
  }
  display.setCursor(79, 20);
  display.setTextSize(2);
  int relAlt = (int)(Altitude - startAlt);
  if (relAlt < 10 && relAlt >= 0) display.print(String(" "));
  if (relAlt < 100 && relAlt >= 0) display.print(String(" "));
  if (relAlt < 1000 && relAlt >= 0) display.print(String(" "));
  display.print(String(relAlt));

  display.fillRect(117, 39, 8, 2, 1);
  display.drawRect(115, 42, 12, 21, 1);
  int batLvl = Battery * 16.0/1023.0;
  if (batLvl > 16) batLvl = 16;
  display.fillRect(117, 61 - batLvl, 8, batLvl, 1);
    
  display.setTextSize(1);
  display.setCursor(0, 37);
  display.print("Packs: ");
  display.print(String((int)package_num));
  display.print(" ");
  display.print(String((int)SendNum));
  display.setCursor(0, 46);
  display.print("P: " + String(Pressure));
  display.print(" T: " + String(Temperature - 2.1, 1));
  display.setCursor(0, 55);
  display.print("G: " + String((int)AirQuality));
  display.print(" PM: " + String(PM25, 1) + " " + String(PM100, 1));
  
  display.display();
  
  compressor.clear();
  package_num++;
  
  logger.addToBuffer(String(rssi) + "_" + String((int)SendNum) + "_" + String(Latitude, 7) + "_" + String(Longitude, 7) + "_" + String(Altitude, 1) + "_" + String(Pressure, 4) + "_" + String(Temperature, 2) + "_" + String((int)AirQuality) + "_" + String(PM25, 1) + "_" + String(PM100, 1) + "_" + String(Humidity, 1) + "_" + String((int)Battery) + "@" + String(millis()) + "\r\n");   
  if (millis() - lastSave >= 2000)
  {
    logger.saveBuffer();
    lastSave = millis();
  }
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

void arrayPush(int arr[], int lenght, int toAdd)
{
  for (int i = 1; i < lenght; i++)
  {
    arr[i - 1] = arr[i];
  }
  arr[lenght - 1] = toAdd;
}
