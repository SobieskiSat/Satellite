#ifndef UTILITIES_H
#define UTILITIES_H

#include "Arduino.h"
#include "Structures/DataPacket.h"
#include "config.h"

namespace SobieskiSat
{
	static void (*LogMessage)(const String msg) = {};
};
	
#endif
