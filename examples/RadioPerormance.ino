include <SobieskiSat.h>

using namespace SobieskiSat;

Radio radio(10, 12, 433.0, Bandwidth_125000_Hz, SpreadingFactor_9, CodingRate_4_8);
Compressor compressor;

void setup()
{
  radio.begin();
  compressor.clear();
  compressor.format = "0_26_Latitude_49.0000000_54.5000000_2_7 26_53_Longitude_14.0699997_24.0900002_2_7 53_67_Altitude_0.0_1000.0_4_1 67_90_Pressure_600.0000_1200.0000_4_4 90_103_Temperature_-20.00_50.00_2_2 103_113_PM25_0.0_100.0_3_1 113_123_PM100_0.0_100.0_3_1 X";

}

void loop()
{
[IN CONSTRUCTION]
}
