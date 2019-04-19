#ifndef CONFIG_H
#define CONFIG_H

// piny komponentów
#define PIN_MQ9 A0
#define PIN_BAT A2
#define PIN_PHR A1
#define PIN_LED 13
#define PIN_DHT 3
#define PIN_SD 11

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
#define MIN_LAT 49f
#define MIN_LON 14.07f
#define MIN_ALT 0f
#define MIN_PRE 600f
#define MIN_TEM -10f
#define MIN_AIR 0
#define MIN_SPS 0f
#define MIN_HUM 0f
#define MIN_BAT 0
	
// maksymalne przewidywane odczyty (sensory wysyłane przez radio)
#define MAX_LAT 52f
#define MAX_LON 24.09f
#define MAX_ALT 5000f
#define MAX_PRE 1100f
#define MAX_TEM 50f
#define MAX_AIR 4096
#define MAX_SPS 102.4f
#define MAX_HUM 100f
#define MAX_BAT 4096
	
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