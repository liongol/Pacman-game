#pragma once
#include "pacman.h"

using namespace std;

Pacman::Pacman()
{
	SetX(14);
	SetY(5);
	SetDirection(eDirection::STAY);
	this->pacman_lives = 3;
	SetShape('@');
}