#ifndef CONFIG_H
#define CONFIG_H

namespace SobieskiSat 
{
// piny komponentów
static const int PIN_MQ9 A0;
static const int PIN_BAT A2;
static const int PIN_PHR A1;
static const int PIN_LED 13;
static const int PIN_DHT 3;
static const int PIN_SD 11;

// konfiguracja powiadomień
static const int LOG_SD 1;
static const int LOG_SERIAL 1;
static const int WAIT_SERIAL 1;

// precyzja zmiennych podczas wysyłania (kompresji) i pisania na USB
static const int PREC_LAT 7;
static const int PREC_LON 7;
static const int PREC_ALT 1;
static const int PREC_PRE 4;
static const int PREC_TEM 2;
static const int PREC_SPS 1;
static const int PREC_HUM 1;
static const int PREC_MPU 7;

// minimalne przewidywane odczyty (sensory wysyłane przez radio)
static const float MIN_SNU 0.0f;
static const float MIN_LAT 49.0f;
static const float MIN_LON 14.07f;
static const float MIN_ALT 0.0f;
static const float MIN_PRE 600.0f;
static const float MIN_TEM -10.0f;
static const float MIN_AIR 0.0f;
static const float MIN_SPS 0.0f;
static const float MIN_HUM 0.0f;
static const float MIN_BAT 0.0f;
	
// maksymalne przewidywane odczyty (sensory wysyłane przez radio)
static const float MAX_SNU 128.0f;
static const float MAX_LAT 52.0f;
static const float MAX_LON 24.09f;
static const float MAX_ALT 6553.0f;
static const float MAX_PRE 1100.0f;
static const float MAX_TEM 50.0f;
static const float MAX_AIR 4096.0f;
static const float MAX_SPS 102.4f;
static const float MAX_HUM 100.0f;
static const float MAX_BAT 4096.0f;
	
// czas między pomiarami, 0 oznacza najmniejszy możliwy
static const int UPD_GPS 0;
static const int UPD_BMP 0;
static const int UPD_MQ9 30;
static const int UPD_SPS 0;
static const int UPD_DHT 0;
static const int UPD_MPU 0;
static const int UPD_BAT 5000;
static const int UPD_PHR 30;

static const int DEL_BEFTRAN 0;
static const int DEL_BETWEENTRAN 0;
static const int DEL_AFTTRAN 0;
static const int DEL_BEFSAVE 0;
static const int DEL_BETWEENSAVE 5000;
static const int DEL_AFTSAVE 0;
static const int DEL_SAVETRAN 30;
	
// opóźnienie pierwszego odczytu (ms)
static const int DEL_SPS 10000;
static const int DEL_DHT 1000;
	
static const int STA_PREINIT 0;
static const int STA_DURINGINIT 1;
static const int STA_INITIALIZED 2;
static const int STA_MISSING 3;
static const int STA_ERROR 4;
static const int STA_DISABLED 5;
static const int STA_PHANTOM 6;

static const int MODE_TX 0;
static const int MODE_RX 1;
};
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