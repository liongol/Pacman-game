#include "entity.h"

using namespace std;

Point Entity::GetPosition() const
{
	return this->position;
}

int Entity::GetX() const
{
	return this->position.x;
}

int Entity::GetY() const
{
	return this->position.y;
}

eDirection Entity::GetDirection() const
{
	return this->direction;
}

char Entity::GetShape() const
{
	return this->shape;
}

void Entity::SetX(const int& x)
{
	this->position.x = x;
}

void Entity::SetY(const int& y)
{
	this->position.y = y;
}

void Entity::SetDirection(const eDirection& dir)
{
	this->direction = dir;
}

void Entity::SetShape(const char& shape)
{
	this->shape = shape;
}

Point Entity::CalcNextPosition(eDirection direction) const
{
	int current_x = this->GetX();
	int current_y = this->GetY();
	Point next_position = { 0 };

	switch (direction)
	{
	case eDirection::LEFT:
	{
		next_position.x = current_x - 1;
		next_position.y = current_y;
		break;
	}
	case eDirection::UP:
	{
		next_position.x = current_x;
		next_position.y = current_y - 1;
		break;
	}
	case eDirection::RIGHT:
	{
		next_position.x = current_x + 1;
		next_position.y = current_y;
		break;
	}
	case eDirection::DOWN:
	{
		next_position.x = current_x;
		next_position.y = current_y + 1;
		break;
	}
	default:
		break;
	}
	return next_position;
}

void Entity::SetToOppositeDirection()
{
	switch (GetDirection())
	{
	case eDirection::LEFT:
	{
		SetDirection(eDirection::RIGHT);
		break;
	}
	case eDirection::UP:
	{
		SetDirection(eDirection::DOWN);
		break;
	}
	case eDirection::RIGHT:
	{
		SetDirection(eDirection::LEFT);
		break;
	}
	case eDirection::DOWN:
	{
		SetDirection(eDirection::UP);
		break;
	}
	}
}

void Entity::Print(char shapeToPrint)
{
	GoToXY(this->GetX(), this->GetY());
	cout << shapeToPrint;
}

void Entity::Move(int newX, int newY, bool isSilent)
{
	if (!isSilent)
		Print(' ');

	SetX(newX);
	SetY(newY);

	if (!isSilent)
		Print(shape);
}

void Entity::HandleStay(bool isSilent) {
	SetDirection(eDirection::STAY);
	if (!isSilent) {
		Print(' ');
		Sleep(80);
		Print(shape);
	}
}

void GoToXY(int x, int y)
{
	HANDLE hConsoleOutput;
	COORD dwCursorPosition;
	cout.flush();
	dwCursorPosition.X = x;
	dwCursorPosition.Y = y;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}