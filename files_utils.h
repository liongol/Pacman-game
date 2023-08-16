#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <list>

#define NUM_MAX_ENTITIES 6

class InputParser
{
private:
	std::vector <std::string> tokens;
public:
	InputParser(int& argc, char** argv);
	bool IsOptionExistInCmd(const std::string& option);
};


class FileManager
{
private:
	std::vector <std::string>stepsString;
public:
	enum eEntityLine //line 0: Pacman moves, line 1: ghost1 moves, ... ,line 4: ghost4 moves, line 6: fruit appearance/disappearance and moves 
	{
		PACMAN_LINE = 0, GHOST1_LINE = 1, GHOST2_LINE = 2, GHOST3_LINE = 3, GHOST4_LINE = 4, FRUIT_LINE = 5
	};

	FileManager() :stepsString(NUM_MAX_ENTITIES) { }
	std::vector<std::string> GetStepsVector();
	void WriteDataToVector(std::string step, int entity_line);
	void WriteStepsToFile(std::string& screen_name);
	void WriteResultToFile(std::vector<std::string> result, std::string& screen_name);
	std::string EditFileName(std::string& screen_name, std::string end);

	std::vector<std::list<std::string>> ReadStepsFromFileToVector(std::string& file_name);
	std::vector<std::list<int>> ReadResultFromFileToVector(std::string& file_name);
};
