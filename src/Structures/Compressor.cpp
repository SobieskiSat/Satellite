#include "Compressor.h"
#include "Arduino.h"
#include <math.h>
#include "../src/Sensors/Sensor.h"
#include "../src/config.h"

using namespace SobieskiSat;

void Compressor::clear()
{
	
	for (int i = 0; i < COMPR_BUFFSIZE; i++)
	{
		data[i] = 0b00000000;
	}
	
	currentBit = 0;
}

void Compressor::attach(char name, float value)
{
	DataPacket packet = getDataPacket(name);
	unsigned int cropped = (unsigned int)((value - packet.bottomLimit) * pow(10, packet.decimals));
	unsigned int spread = (unsigned int)((packet.upperLimit - packet.bottomLimit) * pow(10, packet.decimals));
	int packetBits = 0;
	while (spread > pow(2, packetBits)) { packetBits++; }
	int startIndex = currentBit;
	while (currentBit < startIndex + packetBits)
	{
		bitWrite(data[currentBit / 8], currentBit % 8, bitRead(cropped, currentBit - startIndex));
		currentBit++;
	}
	/*
	if (generateFormat) format += String(startIndex) + "_" + String(index) + "_" + packet.name + "_" +
								  String(packet.bottomLimit, packet.decimals) + "_" + String(packet.upperLimit, packet.decimals) + "_" +
								  String(packet.numbers) + "_" + String(packet.decimals) + " ";
	*/
}

String Compressor::getData()
{
	String toReturn = "";
	int endByte = currentBit / 8;
	if (currentBit % 8 != 0) endByte++;
	for (int i = 0; i < endByte; i++)
	{
		toReturn.concat(data[i]);
	}
	return toReturn;
}

DataPacket Compressor::retrieve(char name)
{
	int startBit;
	int endBit;
	DataPacket found = find(name, startBit, endBit);
	if (receivedLenght > endBit) return DataPacket('N', 0, 0, 0);
	unsigned int unbitten = 0;
	int endByte = endBit / 8;
	bool finished = false;
	bool started = true;
	if (endBit % 8 != 0) endByte++;
	for (int B = startBit / 8; B < endByte && !finished; B++)
	{
		for (int b = 0; b < 8 && !finished; b++)
		{
			if (started)
			{
				b += startBit % 8;
				started = false;
			}
			if ((B * 8 + b) == endBit)
			{
				finished = true;
				break;
			}
			bitWrite(unbitten, (b + B * 8) - startBit, bitRead(data[B], b));
		}
	}
	found.value = (unbitten / pow(10, found.decimals)) + found.bottomLimit;
	return found;
}

void Compressor::download(char name, float& variable)
{
	DataPacket retrieved = retrieve(name);
	if (retrieved.name != 'N') variable = retrieved.value;
}

void Compressor::push(int lenght, char* data_)
{
	for (int i = 0; i < COMPR_BUFFSIZE; i++)
	{
		data[i] = data_[i];
	}
	receivedLenght = lenght;
}

DataPacket Compressor::find(char name, int& startBit, int& endBit)
{
	int ind = 0;
	String current[5];
	int varNum = 0;
	while (ind < COMPR_FORMLENGHT)
	{
		varNum = 0;
		while (COMPR_FORMAT[ind] != ' ')
		{
			if (COMPR_FORMAT[ind] == '_') varNum++;
			else current[varNum] += COMPR_FORMAT[ind];
			ind++;
		}
		// found this name
		if (current[2][0] == name) break;
		
		if (COMPR_FORMAT[ind] == ' ')
		{
			ind++;
			for (int i = 0; i < 7; i++)
			{
				current[i] = "";
			}
		}
	}
	if (COMPR_FORMAT[ind] == 'X')
	{
		return DataPacket('I', 0, 0, 0);
	}
	startBit = current[0].toInt();
	endBit = current[1].toInt();
	return DataPacket(current[2][0], current[3].toFloat(), current[4].toFloat(), 0);
}

DataPacket Compressor::getDataPacket(char name)
{
	switch(name)
	{
		case 'S': return DAT_SNU;
		case 'L': return DAT_LAT;
		case 'l': return DAT_LON;
		case 'A': return DAT_ALT;
		case 'P': return DAT_PRE;
		case 'T': return DAT_TEM;
		case 'Q': return DAT_AIR;
		case '1': return DAT_SPS;
		case '2': return DAT_SPS;
		case '4': return DAT_SPS;
		case '9': return DAT_SPS;
		case 'H': return DAT_HUM;
		case 'B': return DAT_BAT;
		default: return DataPacket('I', 0, 0, 0);
	}
}