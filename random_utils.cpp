#include "random_utils.h"

eDirection RandomDirectionGenerator()
{
	eDirection arr[4] = { eDirection::LEFT , eDirection::RIGHT, eDirection::UP , eDirection::DOWN };
	srand(time(NULL));
	return arr[(rand() % 4)];
}

Point RandomPositionGenerator(int width, int length) {
	Point position;
	position.x = (rand() % width);
	position.y = (rand() % length);
	return position;
}

char RandomCharacterGenerator()
{
	char arr[6] = { '5','6','7','8','9' }; // size=6/5
	srand(time(NULL));
	return arr[(rand() % 5)];
}