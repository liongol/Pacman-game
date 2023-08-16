#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#define BC char(249)	//breadcrumb: ∙
#define WS ' '	//whitespace
#define WALL '#'
#define OUT_OF_BORDER -1
#define ROWS 24
#define COLS 80

struct LengthWidth
{
	int length, width;
};
class Board //creating a board, that can be modified in game_manager
{
private:
	char the_board[ROWS][COLS];
	int num_of_ghosts = 0;
	int ghostsX[4], ghostsY[4];
	int pacmanX, pacmanY;
	std::vector<std::string> screen_names;
	int num_of_screens = 0;
	int starting_bannerX;
	int starting_bannerY;
	LengthWidth length_width_curr_screen;
public:
	char GetCharXY(int x, int y) const; //getting the shape at location (x,y)
	void SetCharXY(int x, int y, char ch); //setting a shape at location (x,y)
	void ReadFromFileToBoard(std::string file_name); //reasing all chars from the file to the board

	int GetNumOfGhosts()const;	//getting the number of ghosts on current screen
	int GetNumOfScreens()const;	//getting the number of screens

	void PrintBoard(int length, int width); //printing the entire board

	const bool EndsWithScreen(const std::string str); //checking if str ends with "screen"
	std::vector<std::string> CreateVectorSortedScreenNames(); //creating a vector that returns all file names in current directory ending with "screen"

	bool AreThereValidFiles();
	std::string IsThereOnlyOnePacman(std::string file_name);
	std::string IsThereABannerInFile(std::string file_name);
	std::string IsBoardSizeValid(std::string file_name);
	std::string IsAValidBoard(std::string file_name);

	int GetGhostX(int ghost_num);	//getting ghost_num's x
	int GetGhostY(int ghost_num);	//getting ghost_num's y

	void SetGhostX(const int& x, int& ghost_num);	//setting ghost_num's x
	void SetGhostY(const int& y, int& ghost_num);	//setting ghost_num's y

	int GetPacmanX() const;	//getting pacman's x
	int GetPacmanY() const;	//getting pacman's y

	int GetBannerX() const; //getting the starting x banner position 
	int GetBannerY() const;	//getting the starting y banner position 

	LengthWidth GetScreenSize() const; //getting the size of current screen
	std::vector<std::string> GetScreenNames() const;//returns a vector, that includes all names of of files that ens with ".screen"
};