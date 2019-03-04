#ifndef SOBIESKISATLIBRARY_PLAYER_H
#define SOBIESKISATLIBRARY_PLAYER_H

#include "Arduino.h"
#include "pitches.h"
#include <utility>

#define song_count 3
#define song_length 10

namespace SobieskiSat
{
	class Player
	{
		public:
		
		void begin(int pin_);
		void update();
		void play(int s);
		void stop();
		
		private:
		
		int pin;
		long timer;
		const int songs[song_count][song_length][2] = {
			{ {nC4, 60}, {0, 60}, {nC4, 60} },
			{ {nC2, 60}, {nB3, 60}, {nC2, 60} },
			{ {nD5, 60}, {0, 60}, {nD5, 60} }
		};
	};
};

#endif