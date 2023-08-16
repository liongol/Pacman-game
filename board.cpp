#pragma once
#include "board.h"
using namespace std;
namespace fs = std::filesystem;

char Board::GetCharXY(int x, int y) const
{
	return this->the_board[y][x]; // the_board[y][x] 
}

void Board::SetCharXY(int x, int y, char ch)
{
	this->the_board[y][x] = ch;
}

void Board::ReadFromFileToBoard(std::string file_name)
{
	int countGhosts = 0;
	ifstream file(file_name);
	if (file.is_open())
	{
		bool flag_banner = false;
		char ch;
		int length = 0, width = 0, max_width = 0;
		while (file.get(ch))
		{
			switch (ch)
			{
			case '\n':
				if (max_width < width)
					max_width = width; //"capturing" max width of screen
				width = 0;
				length++;
				break;
			case WS: //a whitespace in file should be a breadcrumb in game
				SetCharXY(width, length, BC);
				width++;
				break;
			case '%': //'%' in file should be a whitespace in game
				SetCharXY(width, length, OUT_OF_BORDER);
				width++;
				break;
			case '$': //'$' in file should be the initial position of a ghost
				ghostsX[countGhosts] = width;
				ghostsY[countGhosts] = length;
				countGhosts++;
				SetCharXY(width, length, BC);
				width++;
				break;
			case '@': //'@' in file should be the initial position of a ghost
				pacmanX = width;
				pacmanY = length;
				SetCharXY(width, length, WS);
				width++;
				break;
			case '&': //'&' in file should be the initial position of a stats banner
				if (!flag_banner)
				{
					starting_bannerX = width;
					starting_bannerY = length;
					SetCharXY(width, length, OUT_OF_BORDER);
					width++;
					break;
				}
				else
				{
					SetCharXY(width, length, WS);
					width++;
					break;
				}
			case '#':
				SetCharXY(width, length, WALL);
				width++;
				break;
			}
		}
		file.close();
		this->num_of_ghosts = countGhosts;
		this->length_width_curr_screen.length = length;
		this->length_width_curr_screen.width = max_width;
	}
	else cout << "Unable to open file";
}

int Board::GetNumOfGhosts() const
{
	return num_of_ghosts;
}

int Board::GetNumOfScreens() const
{
	return num_of_screens;
}

void Board::PrintBoard(int length, int width)
{
	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < width; j++) {
			if (GetCharXY(j, i) == OUT_OF_BORDER)
				cout << WS;
			else
				cout << this->GetCharXY(j, i);
		}
		cout << endl;
	}
}

const bool Board::EndsWithScreen(const std::string str) //checking if a particular string ends with "screen"
{
	string screens = "screen";
	int str_size = str.size();
	if (str_size < 6) //the string "screen" contains 6 chareacters
		return false;
	string sub_string = str.substr(str_size - 6, str_size); //"cutting" last 6 chars for comparison
	if (screens != sub_string)
		return false;
	return true;
}

std::vector<std::string> Board::CreateVectorSortedScreenNames() //returning a sorted vector, containing all the file names that end with "screen"
{
	vector<string> screen_files;
	string str;
	int count = 0;
	for (const auto& entry : fs::directory_iterator("."))
	{
		if (EndsWithScreen(entry.path().generic_string())) {
			screen_files.push_back(entry.path().generic_string().erase(0, 2));
			count++;
		}
	}
	sort(screen_files.begin(), screen_files.end());
	this->num_of_screens = count;
	this->screen_names = screen_files;
	return this->screen_names;
}

bool Board::AreThereValidFiles()
{
	if (!this->num_of_screens) { //the returned vector is empty, therefore no files with the ending "screen"
		return false;
	}
	return true;
}

std::string Board::IsThereOnlyOnePacman(std::string file_name)
{
	ifstream file(file_name);
	if (file.is_open())
	{
		int pacmans_counter = 0;
		char ch = 't';
		while (file.get(ch))
		{
			if (ch == '@')
				pacmans_counter++;
		}
		file.close();
		if (pacmans_counter == 1)
			return "no_problem";
		else
			return "there is no pacman or more than one pacman in file";
	}
	else
		cout << "File can't be opened";
}

std::string Board::IsThereABannerInFile(std::string file_name)
{
	ifstream file(file_name);
	if (file.is_open())
	{
		int ampersand_counter = 0;
		char ch = 't';
		while (file.get(ch))
		{
			if (ch == '&')
				ampersand_counter++;
		}
		file.close();
		if (ampersand_counter >= 1)
			return "no_problem";
		else
			return "there is no stats banner in file";
	}
	else
		cout << "File can't be opened";
}
std::string Board::IsBoardSizeValid(std::string file_name)
{
	ifstream file(file_name);
	if (file.is_open())
	{
		int max_col_size = 0, rows = 0;
		std::string str;
		while (getline(file, str))
		{
			if (max_col_size < str.size())
				max_col_size = str.size();
			rows++;
		}
		file.close();
		if (max_col_size <= COLS && rows <= ROWS)
			return "no_problem";
		else
			return "screen size in file is too big";
	}
	else
		cout << "File can't be opened";
}

std::string Board::IsAValidBoard(std::string file_name)
{
	std::string res1 = IsThereOnlyOnePacman(file_name);
	std::string res2 = IsThereABannerInFile(file_name);
	std::string res3 = IsBoardSizeValid(file_name);

	bool is_prob1 = false, is_prob2 = false, is_prob3 = false;
	if (res1 != "no_problem")
		is_prob1 = true;
	if (res2 != "no_problem")
		is_prob2 = true;
	if (res3 != "no_problem")
		is_prob3 = true;
	bool flag_problem = false;
	std::string ret_string = "";
	if (is_prob1)
	{
		ret_string += res1 + " & ";
		flag_problem = true;
	}
	if (is_prob2)
	{
		ret_string += res2 + " & ";
		flag_problem = true;
	}
	if (is_prob3)
	{
		ret_string += res3 + " & ";
		flag_problem = true;
	}

	if (!flag_problem)
		return "valid_screen";
	else
		return ret_string.erase(ret_string.size() - 3, ret_string.size()); //cutting the last " & "
}
int Board::GetGhostX(int ghost_num)
{
	return this->ghostsX[ghost_num];
}
int Board::GetGhostY(int ghost_num)
{
	return this->ghostsY[ghost_num];
}
void Board::SetGhostX(const int& x, int& ghost_num)
{
	this->ghostsX[ghost_num] = x;
}
void Board::SetGhostY(const int& y, int& ghost_num)
{
	this->ghostsY[ghost_num] = y;
}

int Board::GetPacmanX() const
{
	return this->pacmanX;
}
int Board::GetPacmanY() const
{
	return this->pacmanY;
}

int Board::GetBannerX() const
{
	return this->starting_bannerX;
}

int Board::GetBannerY() const
{
	return this->starting_bannerY;
}

LengthWidth Board::GetScreenSize() const
{
	return this->length_width_curr_screen;
}

std::vector<std::string> Board::GetScreenNames() const
{
	return this->screen_names;
}