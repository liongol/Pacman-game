#pragma once
#include <iostream>
#include "entity.h"
#include "board.h"

eDirection RandomDirectionGenerator();

Point RandomPositionGenerator(int width, int length);

char RandomCharacterGenerator();