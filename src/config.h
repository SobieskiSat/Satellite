#ifndef CONFIG_H
#define CONFIG_H

#include "Structures/DataPacket.h"
using namespace SobieskiSat;

#define COMPR_FORMAT "0_8_S_0_128_0 8_24_L_49.0_52.0_7 24_40_l_14.07_24.09_7 40_56_A_0_6553_1 56_69_P_600_1100_5 69_84_T_-10.00_50.00_2 84_97_Q_0_4096_0 97_108_1_0.0_102.4_1 108_119_2_0.0_102.4_1 119_130_4_0.0_102.4_1 130_141_9_0.0_102.4_1 141_151_H_0.0_100.0_1 151_164_B_0_4096_0"
#define COMPR_FORMLENGHT 259
#define COMPR_BUFFSIZE 21
#define COMPR_NAMECHAIN "S L l A P T Q 1 2 4 9 H B"

// piny komponentów
#define PIN_MQ9 A0
#define PIN_BAT A4
#define PIN_PHR A2
#define PIN_LED 13
#define PIN_DHT 3
#define PIN_SD 11

// konfiguracja powiadomień
#define LOG_SD 1
#define LOG_SERIAL 1
#define WAIT_SERIAL 1

// precyzja zmiennych podczas wysyłania (kompresji) i pisania na USB
#define PREC_LAT 7
#define PREC_LON 7
#define PREC_ALT 1
#define PREC_PRE 4
#define PREC_TEM 2
#define PREC_SPS 1
#define PREC_HUM 1
#define PREC_MPU 7

// minimalne przewidywane odczyty (sensory wysyłane przez radio)
#define MIN_SNU 0.0f
#define MIN_LAT 49.0f
#define MIN_LON 14.07f
#define MIN_ALT 0.0f
#define MIN_PRE 600.0f
#define MIN_TEM -10.0f
#define MIN_AIR 0.0f
#define MIN_SPS 0.0f
#define MIN_HUM 0.0f
#define MIN_BAT 0.0f
	
// maksymalne przewidywane odczyty (sensory wysyłane przez radio)
#define MAX_SNU 128.0f
#define MAX_LAT 52.0f
#define MAX_LON 24.09f
#define MAX_ALT 6553.0f
#define MAX_PRE 1100.0f
#define MAX_TEM 50.0f
#define MAX_AIR 4096.0f
#define MAX_SPS 102.4f
#define MAX_HUM 100.0f
#define MAX_BAT 4096.0f
	
// czas między pomiarami, 0 oznacza najmniejszy możliwy
#define UPD_GPS 0
#define UPD_BMP 0
#define UPD_MQ9 30
#define UPD_SPS 0
#define UPD_DHT 0
#define UPD_MPU 0
#define UPD_BAT 5000
#define UPD_PHR 30

#define DEL_BEFTRAN 0
#define DEL_BETWEENTRAN 0
#define DEL_AFTTRAN 0
#define DEL_BEFSAVE 0
#define DEL_BETWEENSAVE 5000
#define DEL_AFTSAVE 0
#define DEL_SAVETRAN 30
	
// opóźnienie pierwszego odczytu (ms)
#define DEL_SPS 10000
#define DEL_DHT 1000
	
#define STA_PREINIT 0
#define STA_DURINGINIT 1
#define STA_INITIALIZED 2
#define STA_MISSING 3
#define STA_ERROR 4
#define STA_DISABLED 5
#define STA_PHANTOM 6

#define MODE_TX 0
#define MODE_RX 1

static DataPacket DAT_SNU('S', MIN_SNU, MAX_SNU, 0);
static DataPacket DAT_LAT('L', MIN_LAT, MAX_LAT, PREC_LAT);
static DataPacket DAT_LON('l', MIN_LON, MAX_LON, PREC_LON);
static DataPacket DAT_ALT('A', MIN_ALT, MAX_ALT, PREC_ALT);
static DataPacket DAT_PRE('P', MIN_PRE, MAX_PRE, PREC_PRE);
static DataPacket DAT_TEM('T', MIN_TEM, MAX_TEM, PREC_TEM);
static DataPacket DAT_AIR('Q', MIN_AIR, MAX_AIR, 0);
static DataPacket DAT_SPS('1', MIN_SPS, MAX_SPS, PREC_SPS);
static DataPacket DAT_HUM('H', MIN_HUM, MAX_HUM, PREC_HUM);
static DataPacket DAT_BAT('B', MIN_BAT, MAX_BAT, 0);

/*
enum class SensorStatus
{
	PreInit = 0,
	DuringInit = 1,
	Initialized = 2,
	Missing = 3,
	Error = 4,
	Disabled = 5, 
	Phantom = 6
};
*/
	
#endif
