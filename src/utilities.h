#ifndef UTILITIES_H
#define UTILITIES_H

#include "Arduino.h"
#include "Structures/DataPacket.h"
#include "config.h"

namespace SobieskiSat
{
	static void (*LogMessage)(const String msg) = {};

	inline static int BitsToBytes(const int bitCount)
	{
		int bytes = 0;
		while (pow(2, bytes) < bitCount) bytes++;
		return bytes;
	};

	inline static String Element(const String toSeparate, const char separator, const int targetIndex)
	{
		String temp = "";
		int index = 0;
		int separatorCount = 0;
		
		while (index < toSeparate.length())
		{
			if (toSeparate[index] == separator)
			{
				if (separatorCount == targetIndex) return temp; // może powodować problemy
				
				separatorCount++;
				temp = "";
			}
			else temp += toSeparate[index];
			
			if (separatorCount == targetIndex && index == toSeparate.length() - 1) return temp;
			
			index++;
		}
		
		//LogMessage("[Error] In file: //src//utilities.h, targetIndex was out of range. Inputs: {" + toSeparate + "} {" + String(separator) + "} {" + String(targetIndex) + "}");
		return "InvalidIndex";
	};
	
	inline static int ElementCount(String toSeparate, const char separator)
	{
		int count = 0;
		for (int i = 0; i < toSeparate.length(); i++)
		{
			if (toSeparate[i] == separator) count++;
		}
		if (toSeparate[toSeparate.length() - 1] != separator) count++;
		return count;
	}

	inline static DataPacket FindDataPacket(const String variableName)
	{
		if (variableName == "SNU") 	return DataPacket("SNU", 	MIN_SNU, MAX_SNU, 0);
		if (variableName == "LAT") 	return DataPacket("LAT", 	MIN_LAT, MAX_LAT, PREC_LAT);
		if (variableName == "LON")	return DataPacket("LON", 	MIN_LON, MAX_LON, PREC_LON);
		if (variableName == "ALT") 	return DataPacket("ALT", 	MIN_ALT, MAX_ALT, PREC_ALT);
		if (variableName == "PRE")	return DataPacket("PRE", 	MIN_PRE, MAX_PRE, PREC_PRE);
		if (variableName == "TEM")	return DataPacket("TEM",	MIN_TEM, MAX_TEM, PREC_TEM);
		if (variableName == "AIR")	return DataPacket("AIR",	MIN_AIR, MAX_AIR, 0);
		if (variableName == "PM10")	return DataPacket("PM10", 	MIN_SPS, MAX_SPS, PREC_SPS);
		if (variableName == "PM25")	return DataPacket("PM25", 	MIN_SPS, MAX_SPS, PREC_SPS);
		if (variableName == "PM40")	return DataPacket("PM40", 	MIN_SPS, MAX_SPS, PREC_SPS);
		if (variableName == "PM100")return DataPacket("PM100", 	MIN_SPS, MAX_SPS, PREC_SPS);
		if (variableName == "HUM")	return DataPacket("HUM", 	MIN_HUM, MAX_HUM, PREC_HUM);
		if (variableName == "BAT")	return DataPacket("BAT",	MIN_BAT, MAX_BAT, 0);
		else return DataPacket("INV", 0, 0, 0);
	}
};
	
#endif
