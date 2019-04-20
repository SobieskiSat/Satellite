#include "Compressor.h"
#include "Arduino.h"
#include <math.h>
#include "../src/Sensors/Sensor.h"
#include "../src/utilities.h"

using namespace SobieskiSat;

void Compressor::clear()
{
	for (int i = 0; i < 256; i++)
	{
		data[i] = 0b00000000;
	}
	currentBit = 0;
	currentFormat = "empty";
	receivedLenght = 0;
}

// ####################### COMPRESSION #########################

void Compressor::begin()
{
	Transmitter = false;
}

// Zwraca format na podstawie zmiennych podanych w 'nameChain'
String Compressor::generateFormat(String particularNameChain)
{
	// nameChain: PackNo Laitude Longitude Altitude...
	// return: ID formatu (lenght)~START_STOP_NAME_MAX_MIN_PREC .. START_STOP_NAME_MAX_MIN_PREC ;
	int namesCount = ElementCount(particularNameChain, ' ');
	int lenght = 0;
	for (int i = 0; i < namesCount; i++)
	{
		lenght += FindDataPacket(Element(particularNameChain, ' ', i)).getBitCount();
	}
	
	String toReturn = "";
	int bitIndex = 0;
	
	toReturn += String(lenght, 0) + "~";
	for (int i = 1; i < namesCount; i++)
	{
		String name = Element(particularNameChain, ' ', i);
		DataPacket prototype = FindDataPacket(name);
		toReturn += String(bitIndex, 0) + "_" + String(bitIndex + prototype.getBitCount(), 0) + "_";
		toReturn += prototype.toString();
		bitIndex += prototype.getBitCount();
	}
	
	return toReturn;
}

void Compressor::attach(String name, float value) // zmienić
{
	DataPacket prototype = FindDataPacket(name);
	unsigned int absolute = (unsigned int)((value - prototype.bottomLimit) * pow(10, prototype.decimals) + 0.5f); // 0.5f czy jest tak na pewno?/???????????
	int openingBit = currentBit;
	
	while (currentBit < openingBit + prototype.getBitCount())
	{
		bitWrite(data[currentBit / 8], currentBit % 8, bitRead(absolute, currentBit - openingBit));
		currentBit++;
	}
}

// ################# DECOMPRESSION ####################

void Compressor::begin(String formats_)
{
	Transmitter = true;
	formats = formats_;
}

void Compressor::push(int lenght, char *data_)
{
	for (int i = 0; i < BitsToBytes(lenght); i++)
	{
		data[i] = data_[i];
	}
	receivedLenght = lenght;
	matchFormat();
}

DataPacket Compressor::retrieve(String name)
{
	int openingBit;
	int closingBit;
	DataPacket found = getDataPacketLocation(name, openingBit, closingBit);
	
	int closingByte = BitsToBytes(closingBit);
	bool finished = false;
	bool makeOffset = true;
	
	unsigned int absolute = 0;
	for (int B = BitsToBytes(openingBit); B <= closingByte && !finished; B++)
	{
		for (int b = 0; b < 8 && !finished; b++)
		{
			if (makeOffset)
			{
				b += openingBit % 8; // nie zawsze zaczyna się na początku bajtu
				makeOffset = false;		// wskazuje że zakończyło przesuwanie wskaźnika
			}
			if ((B * 8 + b) == closingBit)
			{
				finished = true;
				break;
			}
			bitWrite(absolute, (b + B * 8) - openingBit, bitRead(data[B], b));
		}
	}
	
	found.value = (absolute / pow(10, found.decimals)) + found.bottomLimit;
	return found;
}

DataPacket Compressor::getDataPacketLocation(String name, int& openingBit, int& closingBit)
{
	// definition: START_STOP_NAME_MAX_MIN_PREC
	
	int elementCount = ElementCount(currentFormat, ' ');
	for (int i = 0; i < elementCount; i++)
	{
		String definitionCandidate = Element(currentFormat, ' ', i);
		String elementName = Element(definitionCandidate, '_', 2);
		if (elementName == name)
		{
			openingBit = 	  Element(definitionCandidate, '_', 0).toInt();
			closingBit = 	  Element(definitionCandidate, '_', 1).toInt();
			return DataPacket(Element(definitionCandidate, '_', 2),
							  Element(definitionCandidate, '_', 3).toFloat(),
							  Element(definitionCandidate, '_', 4).toFloat(),
							  Element(definitionCandidate, '_', 5).toInt()); // bardzo niewydajne!!!!!!!!!!!!!!!
		}
	}
	
	LogMessage("[Error] In file: //src//Structures//Compressor.cpp - getDataPacketLocation, DataPacket could not be found inside format. Input: {" + name + "}");
	return DataPacket("InvalidName", 0, 0, 0);
}

void Compressor::download(String name, float& variable)
{
	variable = retrieve(name).value;
}

void Compressor::matchFormat()
{
	int formatCount = ElementCount(formats, '|'); // do zmiany char -> String jako separator
	for (int i = 0; i < formatCount; i++)
	{
		String formatCandidate = Element(formats, '|', i);
		int candidateLenght = Element(formatCandidate, '~', 0).toInt();
		if (receivedLenght == candidateLenght) currentFormat = Element(formatCandidate, '~', 1);
	}
	
	LogMessage("[Error] In file: //src//Structures//Compressor.cpp - matchFormat, No format could be matched to received lenght.");
	currentFormat = "empty";
}