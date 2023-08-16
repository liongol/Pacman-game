#pragma once
#include <iostream>
#include <Windows.h>

enum class eDirection { LEFT = 'A', RIGHT = 'D', UP = 'W', DOWN = 'X', STAY = 'S' };

struct Point
{
	int x, y;
};

class Entity
{
private:
	Point position;
	char shape;
	eDirection direction;
public:
	Point GetPosition() const;
	int GetX() const;
	int GetY() const;
	eDirection GetDirection() const;
	char GetShape() const;

	void SetX(const int& x);
	void SetY(const int& y);
	void SetDirection(const eDirection& dir);
	void SetShape(const char& shape);

	Point CalcNextPosition(eDirection direction) const;
	void SetToOppositeDirection();

	void Print(char shapeToPrint);
	void Move(int newX, int newY, bool isSilent);
	void HandleStay(bool isSilent);
};


void GoToXY(int x, int y);