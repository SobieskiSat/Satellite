#include "Compressor.h"
#include "Arduino.h"
#include <math.h>
#include "../src/utilities.h"

using namespace SobieskiSat;

void Compressor::clear()
{
	for (int i = 0; i < 256; i++)
	{
		data[i] = 0b00000000;
	}
	currentBit = 0;
	receivedLenght = 0;
}

// ####################### COMPRESSION #########################

void Compressor::begin(int mode)
{
	if (mode == MODE_RX)
	{
		Transmitter = false;
		format = "";
		generateFormat();
	}
	else { Transmitter = true; }
}

// Zwraca format na podstawie zmiennych podanych w 'nameChain'
void Compressor::generateFormat()
{
	// nameChain: PackNo Laitude Longitude Altitude...
	// return: ID formatu (lenght)~START_STOP_NAME_MAX_MIN_PREC .. START_STOP_NAME_MAX_MIN_PREC;
	int namesCount = ElementCount(NAMECHAIN, ' ');
	formatBitLenght = 0;
	for (int i = 0; i < namesCount; i++)
	{
		formatBitLenght += FindDataPacket(Element(NAMECHAIN, ' ', i)).getBitCount();
	}
	formatByteLenght = BitsToBytes(formatBitLenght);
	
	int bitIndex = 0;
	for (int i = 0; i < namesCount; i++)
	{
		String name = Element(NAMECHAIN, ' ', i);
		DataPacket prototype = FindDataPacket(name);
		format += String(bitIndex);
		format += "_";
		format += String(bitIndex + prototype.getBitCount());
		format += "_";
		format += prototype.toString();
		if (i != namesCount - 1) format += " ";
		bitIndex += prototype.getBitCount();
	}
	
	chainElements = ElementCount(format, ' ');
}

void Compressor::attach(String name, float value) // zmienić
{
	DataPacket prototype = FindDataPacket(name);
	unsigned int absolute = (unsigned int)((value - prototype.bottomLimit) * pow(10, prototype.decimals)); // 0.5f czy jest tak na pewno?/???????????
	int openingBit = currentBit;
	
	while (currentBit < openingBit + prototype.getBitCount())
	{
		bitWrite(data[currentBit / 8], currentBit % 8, bitRead(absolute, currentBit - openingBit));
		currentBit++;
	}
}

String Compressor::getData()
{
	String toRetrun = "";
	for (int i = 0; i < formatByteLenght; i++)
	{
		toRetrun += data[i];
	}
	return toRetrun;
}

// ################# DECOMPRESSION ####################

void Compressor::push(int lenght, char *data_)
{
	for (int i = 0; i < BitsToBytes(lenght); i++)
	{
		data[i] = data_[i];
	}
	receivedLenght = lenght;
}

DataPacket Compressor::retrieve(String name)
{	
	int openingBit;
	int closingBit;
	DataPacket found = getDataPacketLocation(name, openingBit, closingBit);
	if (closingBit >= receivedLenght) return DataPacket("NRC", 0, 0, 0);
	
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
	for (int i = 0; i < chainElements; i++)
	{
		String definitionCandidate = Element(format, ' ', i);
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
	
	//LogMessage("[Error] In file: //src//Structures//Compressor.cpp - getDataPacketLocation, DataPacket could not be found inside format. Input: {" + name + "}");
	return DataPacket("INV", 0, 0, 0);
}

void Compressor::download(String name, float& variable)
{
	DataPacket retrieved = retrieve(name);
	if (retrieved.name != "NRC") variable = retrieved.value;
}

/*
void Compressor::matchFormat()
{
	int candidateLenght = 0; // in bits
	candidateLenght += FindDataPacket("SNU").getBitCount();
	candidateLenght += FindDataPacket("LAT").getBitCount();
	candidateLenght += FindDataPacket("LON").getBitCount();
	candidateLenght += FindDataPacket("ALT").getBitCount();
	candidateLenght += FindDataPacket("PRE").getBitCount();
	candidateLenght += FindDataPacket("TEM").getBitCount();
	candidateLenght += FindDataPacket("AIR").getBitCount(); // minimal format
	
	for (int i = 7; i < chainElements; i++)
	{
		if (candidateLenght == receivedLenght)
	}
	
	LogMessage("[Error] In file: //src//Structures//Compressor.cpp - matchFormat, No format could be matched to received lenght.");
}
*/