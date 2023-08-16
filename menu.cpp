#include "menu.h"
using namespace std;

void PrintMenu()
{
	cout << "(1) Start a new game" << endl;
	cout << "(8) Present instructions and keys" << endl;
	cout << "(9) EXIT" << endl;
}
void PrintInstructions()
{
	cout << "The objective is eating all breadcrumbs (" << char(249) << ") on the screen, while avoiding hitting the ghosts" << endl << endl;
	cout << "To control Pacman's (@) movement:" << endl;
	cout << "		W/w: up" << endl;
	cout << "		X/x: down" << endl;
	cout << "		D/d: right" << endl;
	cout << "		A/a: left" << endl;
	cout << "		S/s: stay (pause movement)" << endl << endl;
	cout << "To pause/unpause the game, press \"Esc\"" << endl;
	cout << "In the lower part of the screen, you'll see the number of lives that are left and the score" << endl;
	cout << "Press ENTER to exit the instructions" << endl;
}

void PrintLevelMenu()
{
	cout << "Select level:" << endl;
	cout << "(a) BEST\t(b) GOOD\t(c) NOVICE" << endl;
}

void PrintScreenMenu()
{
	cout << "Select auto/manual screens:" << endl;
	cout << "(a) Auto\t(b) Manual" << endl;
}