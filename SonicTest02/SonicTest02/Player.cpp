#include "Player.h"

Player::Player() {

}

void Player::setCoord(int pos[3]) {
	for (int i = 0; i < 3; i++)
		coord[i] = pos[i];
}


Player::Player(int pos[3]) {
	setCoord(pos);
}

Player::~Player() {
	delete[] coord;
}