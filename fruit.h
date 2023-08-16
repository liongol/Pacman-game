#pragma once
#include "entity.h"
#include "random_utils.h"


class Fruit : public Entity
{
private:

public:
	enum class eAppearance { START = 5, STOP = 30 };
	Fruit();
	void initFruit();
};
