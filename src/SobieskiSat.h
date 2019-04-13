#ifndef SOBIESKISATLIBRARY_H
#define SOBIESKISATLIBRARY_H

#include "Components/CanSatKitRadio.h"
#include "Components/fifo.h"
#include "Components/Logger.h"
#include "Sensors/Sensors.h"
#include "Sensors/Sensor.h"
#include "Structures/Clock.h"
#include "Structures/Time.h"
#include "Arduino.h"
#include "Structures/Compressor.h"
#include "Music/Player.h"

namespace SobieskiSat {
	
	// piny komponentów
	#define PIN_MQ9 A0
	#define PIN_BAT A1
	#define PIN_PHT A2
	#define PIN_LED 13
	
	// precyzja podczas wysyłania i pisania na USB
	#define PREC_LAT 7
	#define PREC_LON 7
	#define PREC_ALT 1
	#define PREC_PRE 4
	#define PREC_TEM 2
	#define PREC_SPS 1
	#define PREC_HUM 1
	
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
	#define UPD_BAT 5000
	#define UPD_PHT 30
	
};

#endif