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
            433.0,                  // frequency in MHz
            Bandwidth_125000_Hz,    // bandwidth - check with CanSat regulations to set allowed value
            SpreadingFactor_9,      // see provided presentations to determine which setting is the best
            CodingRate_4_8);        // see provided presentations to determine which setting is the best
Compressor compressor;
bool led_state = true;

int package_num=0; //zmienna przechowująca liczbę pakietów odebranych (!)

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
  
  compressor.clear();
  compressor.format = "0_26_Latitude_49.0000000_54.5000000_2_7 26_53_Longitude_14.0699997_24.0900002_2_7 53_67_Altitude_0.0_1000.0_4_1 67_90_Pressure_600.0000_1200.0000_4_4 90_103_Temperature_-20.00_50.00_2_2 103_113_PM25_0.0_100.0_3_1 113_123_PM100_0.0_100.0_3_1 X";

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

  int rssi = radio.get_rssi_last();
  SerialUSB.println(String(rssi) + "_" +
                    String(compressor.retrieve("Latitude").value, compressor.retrieve("Latitude").decimals) + "_" + 
                    String(compressor.retrieve("Longitude").value, compressor.retrieve("Longitude").decimals) + "_" +
                    String(compressor.retrieve("Altitude").value, compressor.retrieve("Altitude").decimals) + "_" +
                    String(compressor.retrieve("Pressure").value, compressor.retrieve("Pressure").decimals) + "_" +
                    String(compressor.retrieve("Temperature").value, compressor.retrieve("Temperature").decimals) + "_" +
                    String(compressor.retrieve("PM25").value, compressor.retrieve("PM25").decimals) + "_" +
                    String(compressor.retrieve("PM100").value, compressor.retrieve("PM100").decimals));
  
  //SerialUSB.println(String(compressor.data));
  //OLED PRINT  

  display.clearDisplay();
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setCursor(0, 0);
  display.print("RSSI: ");
  display.println(rssi);
  display.print("H: ");
  display.println(String(compressor.retrieve("Altitude").value));

  display.print("P: ");
  display.println(String(int(compressor retrieve("PackNo").value)+ " " + String(package_num) + " " + String(package_num % 32));
                        );

  display.print("T: ");
  display.println(String(compressor.retrieve("Temperature").value));
  
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
