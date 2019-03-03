#include "Compressor.h"
#include "Arduino.h"
#include <math.h>

using namespace SobieskiSat;

void Compressor::clear()
{
	for (int i = 0; i < packetSize; i++)
	{
		data[i] = 0b00000000;
	}
	index = 0;
	if (generateFormat) format += "X";
	generateFormat = false;
}

void Compressor::attach(DataPacket packet)
{
	unsigned int cropped = (unsigned int)((packet.value - packet.bottomLimit) * pow(10, packet.decimals));
	unsigned int spread = (unsigned int)((packet.upperLimit - packet.bottomLimit) * pow(10, packet.decimals));
	int packetBits = 0;
	while (spread > pow(2, packetBits)) { packetBits++; }
	int startIndex = index;
	while (index < startIndex + packetBits)
	{
		bitWrite(data[index / 8], index % 8, bitRead(cropped, index - startIndex));
		index++;
	}
	if (generateFormat) format += String(startIndex) + "_" + String(index) + "_" + packet.name + "_" +
								  String(packet.bottomLimit, packet.decimals) + "_" + String(packet.upperLimit, packet.decimals) + "_" +
								  String(packet.numbers) + "_" + String(packet.decimals) + " ";
}

DataPacket Compressor::retrieve(String name)
{
	if (format != "")
	{
		int startBit;
		int endBit;
		DataPacket found = find(name, startBit, endBit);
		unsigned int unbitten = 0;
		int endByte = endBit / 8;
		bool finished = false;
		bool started = true;
		if (endByte % 8 != 0) endByte += 8 - (endByte % 8);
		for (int B = startBit / 8; B <= endByte && !finished; B++)
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
	else
	{
		SerialUSB.println("Unable to decode! Format do not exist");
	}
}

DataPacket Compressor::find(String name, int& startBit, int& endBit)
{
	int ind = 0;
	String current[7];
	int varNum = 0;
	while (format[ind] != 'X')
	{
		varNum = 0;
		while (format[ind] != ' ')
		{
			if (format[ind] == '_') varNum++;
			else current[varNum] += format[ind];
			ind++;
		}
		// found this name
		if (current[2] == name) break;
		
		if (format[ind] == ' ')
		{
			ind++;
			for (int i = 0; i < 7; i++)
			{
				current[i] = "";
			}
		}
	}
	startBit = current[0].toInt();
	endBit = current[1].toInt();
	return DataPacket(current[2], current[3].toFloat(), current[4].toFloat(), current[5].toInt(), current[6].toInt(), -1);
}