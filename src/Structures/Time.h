#ifndef SOBIESKISATLIBRARY_DATETIME_H
#define SOBIESKISATLIBRARY_DATETIME_H

#include "Arduino.h"

namespace SobieskiSat
{
	struct Time
	{
		int Milliseconds;
		int Seconds;
		int Minutes;
		int Hour;
		int Day;
		int Month;
		int Year;
		
		Time operator+(int milliseconds)
		{
			Milliseconds += milliseconds;
			if (overflowAdd(Milliseconds, Seconds, 999))
			if (overflowAdd(Seconds, Minutes, 59))
			if (overflowAdd(Minutes, Hour, 59))
				overflowAdd(Hour, Day, 23);
			return *this;
		}
		
		bool overflowAdd(int &first, int &second, int limit)
		{
			bool toReturn = false;
			while (first > limit)
			{
				toReturn = true;
				first -= limit + 1;
				second++;
			}
			return toReturn;
		}
		
		String getString()
		{	
			String toReturn = "";
			toReturn += intToString(Day, 2);
			toReturn += intToString(Month, 2);
			toReturn += intToString(Year, 2);
			toReturn += intToString(Hour, 2);
			toReturn += intToString(Minutes, 2);
			toReturn += intToString(Seconds, 2);
			toReturn += intToString(Milliseconds, 3);	
			return toReturn;
		}
		
			
		String intToString(int value, int dec_places)
		{
			String toReturn = String(value, DEC);
			while (toReturn.length() < dec_places)
			{
				toReturn = "0" + toReturn;
			}
			return toReturn;
		}
	};
};
	   
#endif