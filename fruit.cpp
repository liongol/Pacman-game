#include "fruit.h"

using namespace std;

void Fruit::initFruit() {
	SetShape(RandomCharacterGenerator());
	SetDirection(RandomDirectionGenerator());
}

Fruit::Fruit() {
	initFruit();
}