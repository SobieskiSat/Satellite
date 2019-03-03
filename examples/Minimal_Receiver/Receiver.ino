#include <SobieskiSat.h>

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

void setup() {
  SerialUSB.begin(115200);

  // start radio module  
  radio.begin();
  compressor.clear();
  compressor.format = "0_27_Latitude_-24.0900002_-14.0699997_2_7 27_53_Longitude_49.0000000_54.5000000_2_7 53_67_Altitude_0.0_1000.0_4_1 67_90_Pressure_600.0000_1200.0000_4_4 90_103_Temperature_-20.00_50.00_2_2 103_113_PM2_5_0.0_100.0_3_1 113_123_PM10_0_0.0_100.0_3_1";
}

void loop() {
  // prepare empty space for received frame
  // maximum length is maximum frame length + null termination
  // 255 + 1 byte = 256 bytes
  char data[256];

  // receive data and save it to string
  radio.receive(data);

  for (int i = 0; i < 47; i++)
  {
    compressor.data[i] = data[i];
  }
  
  // get and print signal level (rssi)
  SerialUSB.println(radio.get_rssi_last() + "_" +
                    String(compressor.retrieve("Latitude").value, compressor.retrieve("Latitude").decimals) + "_" + 
                    String(compressor.retrieve("Longitude").value, compressor.retrieve("Longitude").decimals) + "_" +
                    String(compressor.retrieve("Altitude").value, compressor.retrieve("Altitude").decimals) + "_" +
                    String(compressor.retrieve("Pressure").value, compressor.retrieve("Pressure").decimals) + "_" +
                    String(compressor.retrieve("Temperature").value, compressor.retrieve("Temperature").decimals) + "_" +
                    String(compressor.retrieve("PM2_5").value, compressor.retrieve("PM2_5").decimals) + "_" +
                    String(compressor.retrieve("PM10_0").value, compressor.retrieve("PM10_0").decimals));
}