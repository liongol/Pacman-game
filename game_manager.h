#pragma once
#include "board.h"
#include "menu.h"
#include "fruit.h"
#include "ghost.h"
#include "pacman.h"
#include "files_utils.h"

#define MAX_X 80
#define MIN_X 0
#define BC_POINT 1

class GameManager
{
private:

	enum class eKeyBoardOptions { ENTER = 13, ESC = 27 };
	enum class eMenuOptions { START_GAME = '1', PRINT_INSTUCTIONS = '8', EXIT = '9' };
	enum class eLevel { BEST = 'A', GOOD = 'B', NOVICE = 'C' };

	std::string mode;
	bool isSilent = 0;
	int lives = 3;
	int score = 0;
	int breadCrumbCounter = 0;
	Pacman pacman;
	Ghost ghosts_arr[4] = { 1,2,3,4 };
	Board board;
	Fruit fruit;
	eLevel level;
	bool autoScreen = 0;
	bool gameWon = 0;
	std::string screen;
	std::vector<std::string> screen_names;
	int num_of_screens;
	int curr_screen = 0;
	FileManager fileManager;
	std::vector<std::list<std::string>> stepsFromFile;
	int sleep_time;

public:

	void StartGame(int argc, char** argv); //starting game, from the main menu
	void SelectLevel();
	void SelectScreen();
	int SearchScreen();
	void InitGame();
	int CountBreadCrumb();
	void Run(); //starting the "actual game", after choosing (1) in the main menu
	eDirection GetNextDirectionFromList(FileManager::eEntityLine entityLine);
	std::string GetNextFruitDataFromList();
	void InitGameMode(int argc, char** argv);
	int GetLives() const;
	int GetScore() const;
	int GetBreadCrumbCounter() const;

	eLevel GetLevel() const;
	void SetLevel(eLevel level);

	const void PrintHeadUpDisplay(); //printing the "bar", that includes remaining lives and score

	Pacman GetPacman() const;
	Ghost GetGhost(const int& ghost_num) const;
	Fruit GetFruit() const;
	int GetNumOfScreens() const;
	int GetCurrScreenNumber() const;
	void SetNumberOfScreens(const int num);

	void LiveReduction();
	void ScoreAddition(int toAdd);
	void HandlePacmanMovement(int& moves);
	std::string ConvertDirectionToString(eDirection direction);
	eDirection ConvertStringToDirection(std::string str);
	bool DidPacmanEatFruit();
	void HandleGhostMovement(Ghost& ghost1, int& moves);
	void DetermineGhostNewDirection(Ghost& ghost1, int& moves);

	//LEVELS
	void HandleBestLevel(Ghost& ghost1);

	void SetValidDirection(Entity& entity);

	void ChasePacman(Ghost& ghost1);

	bool IsVertex(Point position);

	void HandleFruitMovement();
	void HandleEscape();
	void SetNewRandomDirection(Entity& entity);

	void SetFruit();

	bool IsPacmanEaten();

	void HandleLiveReduction(); //reducing lives (also resets objects on the screen)
	void HandleGameOver();
	void HandleGameWon();

	bool IsWall(char ch) const; //checking if the character is '#'
	bool IsDirection(char ch) const;
	void ResetGame(); //resetting objects, including lives and score

};