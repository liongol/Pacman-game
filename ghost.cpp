#include "ghost.h"
#include <ctime>
using namespace std;

Ghost::Ghost(int id)
{
	SetX(13);
	SetY(11);
	SetDirection(eDirection::STAY);
	SetShape('$');
	this->id = id;
}

int Ghost::GetId()
{
	return id;
}
