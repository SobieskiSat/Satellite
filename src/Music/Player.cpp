#include "Arduino.h"
#include "Player.h"

using namespace SobieskiSat;

void Player::begin(int pin_)
{
	pin = pin_;
	pinMode(pin, OUTPUT);
}

void Player::play(int s)
{
	tone(pin, songs[s][0][0], songs[s][0][1]);
	delay(250);
	tone(pin, songs[s][1][0], songs[s][1][1]);
	delay(250);
	tone(pin, songs[s][2][0], songs[s][2][1]);
	return;
}

void Player::update()
{
	return;
}