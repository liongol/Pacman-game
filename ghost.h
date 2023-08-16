#pragma once
#include <iostream>
#include <Windows.h>
#include "entity.h"

class Ghost : public Entity
{
private:
	int id;
public:
	int GetId();
	Ghost(int id); //default constructor, initializes the ghost in a particular location
};