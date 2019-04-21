#ifndef CONFIG_H
#define CONFIG_H

// piny komponentów
#define PIN_MQ9 A0
#define PIN_BAT A2
#define PIN_PHR A1
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
#define MIN_SNU 0.0
#define MIN_LAT 49.0
#define MIN_LON 14.07
#define MIN_ALT 0.0
#define MIN_PRE 600.0
#define MIN_TEM -10.0
#define MIN_AIR 0.0
#define MIN_SPS 0.0
#define MIN_HUM 0.0
#define MIN_BAT 0.0
	
// maksymalne przewidywane odczyty (sensory wysyłane przez radio)
#define MAX_SNU 128.0
#define MAX_LAT 52.0
#define MAX_LON 24.09
#define MAX_ALT 6553.0
#define MAX_PRE 1100.0
#define MAX_TEM 50.0
#define MAX_AIR 4096.0
#define MAX_SPS 102.4
#define MAX_HUM 100.0
#define MAX_BAT 4096.0
	
// czas między pomiarami, 0 oznacza najmniejszy możliwy
#define UPD_GPS 0
#define UPD_BMP 0
#define UPD_MQ9 30
#define UPD_SPS 0
#define UPD_DHT 0
#define UPD_MPU 0
#define UPD_BAT 5000
#define UPD_PHR 30
	
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