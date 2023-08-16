#include "game_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

using namespace std;
namespace fs = std::filesystem;

int GameManager::GetLives() const
{
	return this->lives;
}

int GameManager::GetScore() const
{
	return this->score;
}

GameManager::eLevel GameManager::GetLevel() const
{
	return this->level;
}

void GameManager::SetLevel(eLevel level)
{
	this->level = level;
}

int GameManager::GetBreadCrumbCounter() const {
	return this->breadCrumbCounter;
}

Pacman GameManager::GetPacman() const
{
	return this->pacman;
}

Ghost GameManager::GetGhost(const int& ghost_num) const
{
	return this->ghosts_arr[ghost_num];
}

Fruit GameManager::GetFruit() const
{
	return this->fruit;
}

void GameManager::LiveReduction()
{
	this->lives--;
}

int GameManager::GetNumOfScreens() const
{
	return this->num_of_screens;
}

void GameManager::SetNumberOfScreens(const int num)
{
	this->num_of_screens = num;
}

int GameManager::GetCurrScreenNumber() const
{
	return this->curr_screen;
}

void GameManager::ScoreAddition(int toAdd)
{
	this->score += toAdd;
	if (!isSilent) {
		PrintHeadUpDisplay();
	}
}

void GameManager::InitGame()
{
	board.ReadFromFileToBoard(screen_names[curr_screen]);
	this->pacman.SetX(board.GetPacmanX());
	this->pacman.SetY(board.GetPacmanY());
	int starting_banner_x = board.GetBannerX();
	int starting_banner_y = board.GetBannerY();

	this->score = 0;

	for (int i = 0; i < board.GetNumOfGhosts(); i++)
	{
		this->ghosts_arr[i].SetX(board.GetGhostX(i));
		this->ghosts_arr[i].SetY(board.GetGhostY(i));
		SetValidDirection(ghosts_arr[i]);
	}

	this->pacman.SetDirection(eDirection::STAY);

	breadCrumbCounter = CountBreadCrumb();
	SetFruit();
}



int GameManager::CountBreadCrumb() {
	int count = 0;
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (board.GetCharXY(j, i) == BC)
				count++;
		}
	}
	return count;
}

void GameManager::Run()
{
	InitGame();
	int movesCounter = 0;
	vector<string> result(2);
	vector<list<int>> resultList(2);
	if (isSilent) {
		string resultFileName = fileManager.EditFileName(screen_names[curr_screen], "result");
		resultList = fileManager.ReadResultFromFileToVector(resultFileName);
	}
	bool turnToMove = true;
	bool gameStarted = false;
	LengthWidth lw = board.GetScreenSize();

	if (!isSilent) {
		board.PrintBoard(lw.length, lw.width);
		PrintHeadUpDisplay();

		for (int i = 0; i < board.GetNumOfGhosts(); i++) {
			ghosts_arr[i].Print(ghosts_arr->GetShape());
		}

		pacman.Print(pacman.GetShape());
	}

	score += BC_POINT;
	breadCrumbCounter--;
	bool repeat = true;
	int fruitMovesCount = 0, ghostMovesCount = 0;

	while (repeat)
	{
		if (mode == "load") {
			gameStarted = true;
			pacman.SetDirection(GetNextDirectionFromList(FileManager::PACMAN_LINE));
		}
		else if (_kbhit()) {
			char key = toupper(_getch());
			if (!gameStarted && IsDirection(key)) {
				gameStarted = true;
			}
			switch (key)
			{
			case (char)eDirection::DOWN:
				pacman.SetDirection(eDirection::DOWN);
				break;
			case (char)eDirection::UP:
				pacman.SetDirection(eDirection::UP);
				break;
			case (char)eDirection::LEFT:
				pacman.SetDirection(eDirection::LEFT);
				break;
			case (char)eDirection::RIGHT:
				pacman.SetDirection(eDirection::RIGHT);
				break;
			case (char)eDirection::STAY:
				pacman.SetDirection(eDirection::STAY);
				break;
			case (char)eKeyBoardOptions::ESC:
				HandleEscape();
				break;
			default:
				break;
			}
		}

		int i = 0;
		if (turnToMove && gameStarted) {
			while (i < board.GetNumOfGhosts()) {
				HandleGhostMovement(ghosts_arr[i++], ghostMovesCount);
			}
			fruitMovesCount++;
			ghostMovesCount++;
		}

		if (turnToMove && fruitMovesCount > (int)Fruit::eAppearance::START && gameStarted) {
			HandleFruitMovement();
		}

		if (fruitMovesCount == (int)Fruit::eAppearance::STOP) {
			if (!isSilent) {
				fruit.Print(board.GetCharXY(fruit.GetX(), fruit.GetY()));
			}
			fruitMovesCount = 0;
			if (mode != "load") {
				fruit.initFruit();
			}
			SetFruit();
		}

		HandlePacmanMovement(fruitMovesCount);
		if (gameStarted) {
			movesCounter++;
		}

		if (gameStarted && mode == "save") {
			fileManager.WriteDataToVector(ConvertDirectionToString(pacman.GetDirection()), FileManager::PACMAN_LINE);
		}

		turnToMove = !turnToMove;

		if (IsPacmanEaten())
		{
			if (mode == "save") {
				result[0].append(to_string(movesCounter));
				result[0].append(" ");
			}

			LiveReduction();

			if (isSilent) {
				if (movesCounter != resultList[0].front()) {
					cout << "test failed" << endl;
					repeat = false;
				}
				resultList[0].pop_front();
			}
			if (lives <= 0)
			{
				HandleGameOver();
				repeat = false;
				break;
			}
			else
			{
				HandleLiveReduction();
			}

		}

		if (breadCrumbCounter == 0) {
			if (mode == "save") {
				result[1].append(to_string(movesCounter));
			}
			if (movesCounter != resultList[1].front()) {
				cout << "test failed" << endl;
				repeat = false;
			}
			resultList[1].pop_front();
			HandleGameWon();
			repeat = false;
			break;
		}
		Sleep(sleep_time);
	}

	if (mode == "save") {
		fileManager.WriteStepsToFile(screen_names[curr_screen]);
		fileManager.WriteResultToFile(result, screen_names[curr_screen]);
	}
}

eDirection GameManager::GetNextDirectionFromList(FileManager::eEntityLine entityLine) {
	string str = stepsFromFile[entityLine].front();
	stepsFromFile[entityLine].pop_front();
	return ConvertStringToDirection(str);
}

string GameManager::GetNextFruitDataFromList() {
	string str = stepsFromFile[FileManager::FRUIT_LINE].front();
	stepsFromFile[FileManager::FRUIT_LINE].pop_front();
	return str;
}

void GameManager::InitGameMode(int argc, char** argv) {
	InputParser inputParser(argc, argv);

	if (inputParser.IsOptionExistInCmd("-load")) {
		mode = "load"; // define the modes and sleep times ..
		if (inputParser.IsOptionExistInCmd("-silent")) {
			isSilent = true;
			sleep_time = 0;
		}
		else {
			sleep_time = 100;
		}
	}
	else if (inputParser.IsOptionExistInCmd("-save")) {
		mode = "save";
		sleep_time = 200;
	}
	else {
		mode = "simple";
		sleep_time = 200;
	}
}

void GameManager::StartGame(int argc, char** argv)
{
	InitGameMode(argc, argv);

	this->screen_names = board.CreateVectorSortedScreenNames();
	this->SetNumberOfScreens(screen_names.size());
	if (board.AreThereValidFiles()) {
		if (mode == "load") {
			for (int i = 0; i < num_of_screens; i++) {
				string stepsFileName = fileManager.EditFileName(screen_names[i], "steps");
				stepsFromFile = fileManager.ReadStepsFromFileToVector(stepsFileName);
				Run();
			}
		}
		else {
			PrintMenu();
			char menu_select;
			do {
				menu_select = _getch();
				switch (menu_select)
				{
				case (char)eMenuOptions::START_GAME:
					system("CLS");
					SelectLevel();
					if (!autoScreen) {
						SelectScreen();
					}
					Run();
					system("CLS");
					ResetGame();
					PrintMenu();
					break;
				case (char)eMenuOptions::PRINT_INSTUCTIONS:
					system("CLS");
					PrintInstructions();
					while (1) {
						if (_kbhit() && _getch() == (char)eKeyBoardOptions::ENTER) {
							system("CLS");
							PrintMenu();
							break;
						}
					}
				}
				_flushall();
			} while (menu_select != (char)eMenuOptions::EXIT);
			system("CLS");
			cout << "GOODBYE";
		}
	}
	else {
		cout << "No files found. Can't start the game" << endl;
	}
}

void GameManager::SelectLevel() {
	PrintLevelMenu();
	char level_select;
	bool validInput = false;
	do {
		level_select = toupper(_getch());
		if (level_select >= (char)eLevel::BEST && level_select <= (char)eLevel::NOVICE) {
			SetLevel((GameManager::eLevel)level_select);
			validInput = true;
		}
		_flushall();
	} while (!validInput);
	system("CLS");
}

void GameManager::SelectScreen() {
	PrintScreenMenu();
	char screen_select;
	bool validInput = false;
	do {
		screen_select = toupper(_getch());
		if (screen_select == 'A' || screen_select == 'B') {
			autoScreen = true;
			validInput = true;
		}
		if (screen_select == 'B') {
			autoScreen = false;
			cout << "Enter screen name:" << endl;
			cin >> this->screen;
			int index = SearchScreen();
			if (index == -1) {
				cout << "Invald screen name" << endl;
				Sleep(2000);
				system("CLS");
				validInput = false;
				PrintScreenMenu();
			}
			else {
				curr_screen = index;
				validInput = true;
			}
		}
		_flushall();
	} while (!validInput);
	system("CLS");
}

int GameManager::SearchScreen() {
	for (int i = 0; i < board.GetNumOfScreens(); i++) {
		if (this->screen == screen_names[i]) {
			return i;
		}
	}
	return -1;
}

void GameManager::ResetGame() {
	this->board = Board();
	InitGame();
}

void GameManager::HandlePacmanMovement(int& moves)
{
	Point next_position = pacman.CalcNextPosition(pacman.GetDirection());

	if (next_position.x < 0) {
		if (!IsWall(board.GetCharXY(board.GetScreenSize().width - 1, next_position.y)))
			next_position.x = board.GetScreenSize().width - 1;
	}

	if (next_position.x > board.GetScreenSize().width - 1) {
		if (!IsWall(board.GetCharXY(0, next_position.y)))
			next_position.x = 0;
	}

	if (next_position.y > board.GetScreenSize().length - 1) {
		if (!IsWall(board.GetCharXY(next_position.x, board.GetScreenSize().length - 1)))
			next_position.y = 0;
	}

	if (next_position.y < 0) {
		if (!IsWall(board.GetCharXY(next_position.x, board.GetScreenSize().length - 1)))
			next_position.y = board.GetScreenSize().length - 1;
	}

	if (IsWall(this->board.GetCharXY(next_position.x, next_position.y))) {
		pacman.HandleStay(isSilent);
	}

	if (board.GetCharXY(next_position.x, next_position.y) == BC) {
		ScoreAddition(BC_POINT);
		breadCrumbCounter -= BC_POINT;
		board.SetCharXY(next_position.x, next_position.y, WS);
		pacman.Move(next_position.x, next_position.y, isSilent);
	}

	if (board.GetCharXY(next_position.x, next_position.y) == WS) {
		pacman.Move(next_position.x, next_position.y, isSilent);
	}

	if (next_position.x == fruit.GetX() && next_position.y == fruit.GetY()) { //
		ScoreAddition(fruit.GetShape() - '0');
		moves = 0;
		if (mode != "load") {
			fruit.initFruit();
		}
		SetFruit();
	}
}

std::string GameManager::ConvertDirectionToString(eDirection direction) {
	switch (direction)
	{
	case eDirection::LEFT:
		return "LEFT";
	case eDirection::RIGHT:
		return "RIGHT";
	case eDirection::UP:
		return "UP";
	case eDirection::DOWN:
		return "DOWN";
	case eDirection::STAY:
		return "STAY";
	}
}

eDirection GameManager::ConvertStringToDirection(string str) {

	if (str == "LEFT") {
		return eDirection::LEFT;
	}
	else if (str == "RIGHT") {
		return eDirection::RIGHT;
	}
	else if (str == "UP") {
		return eDirection::UP;
	}
	else if (str == "DOWN") {
		return eDirection::DOWN;
	}
	else if (str == "STAY") {
		return eDirection::STAY;
	}
}

bool GameManager::DidPacmanEatFruit()
{
	bool res = false;
	if (pacman.GetX() == fruit.GetX() && pacman.GetY() == fruit.GetY()) {
		res = true;
	}
	return res;
}

bool GameManager::IsPacmanEaten()
{
	bool res = false;
	for (int i = 0; i < board.GetNumOfGhosts(); i++) {
		if (pacman.GetX() == ghosts_arr[i].GetX() && pacman.GetY() == ghosts_arr[i].GetY()) {
			res = true;
		}
	}
	return res;
}

void GameManager::HandleLiveReduction()
{
	if (!isSilent) {
		system("CLS");
		LengthWidth lw = board.GetScreenSize();
		board.PrintBoard(lw.length, lw.width);
		PrintHeadUpDisplay();
	}

	pacman.Move(board.GetPacmanX(), board.GetPacmanY(), isSilent);
	for (int i = 0; i < board.GetNumOfGhosts(); i++) {
		ghosts_arr[i].Move(board.GetGhostX(i), board.GetGhostY(i), isSilent);
	}
}

void GameManager::HandleGameOver()
{
	lives = 3;
	curr_screen = 0;
	autoScreen = false;
	if (!isSilent) {
		system("CLS");
		cout << "Game Over" << endl;
	}
	if (mode != "load") {
		char getter;
		cout << "press any key to continue";
		getter = _getch();
	}
}

void GameManager::HandleGameWon()
{
	curr_screen++;
	if (!isSilent) {
		system("CLS");
		if (autoScreen && curr_screen >= board.GetNumOfScreens()) {
			curr_screen = 0;
			autoScreen = false;
			cout << "Congradulations, you've finished all the screens" << endl;
		}

		cout << "Game Won! ";
	}
	if (mode != "load") {
		cout << "press any key to continue";
		char getter = _getch();
	}
}

void GameManager::HandleGhostMovement(Ghost& ghost1, int& moves)
{
	Point position = { ghost1.GetPosition() };
	Point next_position;

	if (mode != "load") {
		DetermineGhostNewDirection(ghost1, moves);

		next_position = ghost1.CalcNextPosition(ghost1.GetDirection());

		if (IsWall(this->board.GetCharXY(next_position.x, next_position.y)))
		{
			ghost1.HandleStay(isSilent);
			SetValidDirection(ghost1);
			next_position = ghost1.CalcNextPosition(ghost1.GetDirection());
		}

		if (next_position.x > board.GetScreenSize().width - 1) {
			ghost1.HandleStay(isSilent);
			ghost1.SetToOppositeDirection();
			next_position.x = board.GetScreenSize().width - 2;
		}

		if (next_position.x < 0) {
			ghost1.HandleStay(isSilent);
			ghost1.SetToOppositeDirection();
			next_position.x = 1;
		}

		if (next_position.y > board.GetScreenSize().length - 1) {
			ghost1.HandleStay(isSilent);
			ghost1.SetToOppositeDirection();
			next_position.y = board.GetScreenSize().length - 2;
		}

		if (next_position.y < 0) {
			ghost1.HandleStay(isSilent);
			ghost1.SetToOppositeDirection();
			next_position.y = 1;
		}
	}
	else {
		ghost1.SetDirection(GetNextDirectionFromList((FileManager::eEntityLine)ghost1.GetId()));
		next_position = ghost1.CalcNextPosition(ghost1.GetDirection());
	}

	if (board.GetCharXY(position.x, position.y) == BC)
	{
		ghost1.Move(next_position.x, next_position.y, isSilent);
		if (!isSilent) {
			GoToXY(position.x, position.y);
			cout << BC;
		}
	}

	else if (board.GetCharXY(position.x, position.y) == WS) {
		ghost1.Move(next_position.x, next_position.y, isSilent);
	}

	if (mode == "save") {
		fileManager.WriteDataToVector(ConvertDirectionToString(ghost1.GetDirection()), ghost1.GetId());
	}
}

void GameManager::DetermineGhostNewDirection(Ghost& ghost1, int& moves) {
	switch (GetLevel())
	{
	case eLevel::BEST:
		HandleBestLevel(ghost1);
		//handle best..

		break;

	case eLevel::GOOD:

		if (moves % 25 == 0) {
			moves = 0;
		}
		if (moves % 25 < 20) {
			HandleBestLevel(ghost1);
		}
		else if (moves % 25 == 20) {
			SetNewRandomDirection(ghost1);
		}
		break;

	case eLevel::NOVICE:
		if (moves % 20 == 0) {
			SetNewRandomDirection(ghost1);
			moves = 1;
		}
		break;

	default:
		break;
	}
}

void GameManager::HandleBestLevel(Ghost& ghost1) {
	if (IsVertex(ghost1.GetPosition())) {
		ChasePacman(ghost1);
	}
}

void GameManager::SetValidDirection(Entity& entity) {
	eDirection directionsArray[4];
	int i = 0;
	if (!IsWall(board.GetCharXY(entity.GetX(), entity.GetY() + 1))) {
		directionsArray[i++] = eDirection::DOWN;
	}
	if (!IsWall(board.GetCharXY(entity.GetX(), entity.GetY() - 1))) {
		directionsArray[i++] = eDirection::UP;
	}
	if (!IsWall(board.GetCharXY(entity.GetX() + 1, entity.GetY()))) {
		directionsArray[i++] = eDirection::RIGHT;
	}
	if (!IsWall(board.GetCharXY(entity.GetX() - 1, entity.GetY()))) {
		directionsArray[i++] = eDirection::LEFT;
	}

	srand(time(NULL));
	entity.SetDirection(directionsArray[(rand() % i)]);
}

void GameManager::ChasePacman(Ghost& ghost1) {
	eDirection directionsArray[4];
	int i = 0;

	if (!IsWall(board.GetCharXY(ghost1.GetX(), ghost1.GetY() + 1)) && ghost1.GetY() < pacman.GetY()) {
		directionsArray[i++] = eDirection::DOWN;
	}
	if (!IsWall(board.GetCharXY(ghost1.GetX(), ghost1.GetY() - 1)) && ghost1.GetY() > pacman.GetY()) {
		directionsArray[i++] = eDirection::UP;
	}
	if (!IsWall(board.GetCharXY(ghost1.GetX() + 1, ghost1.GetY())) && ghost1.GetX() < pacman.GetX()) {
		directionsArray[i++] = eDirection::RIGHT;
	}
	if (!IsWall(board.GetCharXY(ghost1.GetX() - 1, ghost1.GetY())) && ghost1.GetX() > pacman.GetX()) {
		directionsArray[i++] = eDirection::LEFT;
	}
	if (i != 0) {
		ghost1.SetDirection(directionsArray[(rand() % i)]);
	}
}

bool GameManager::IsVertex(Point position) {
	int wallsCount = 0;
	if (IsWall(board.GetCharXY(position.x, position.y + 1))) {
		wallsCount++;
	}
	if (IsWall(board.GetCharXY(position.x, position.y - 1))) {
		wallsCount++;
	}
	if (IsWall(board.GetCharXY(position.x + 1, position.y))) {
		wallsCount++;
	}
	if (IsWall(board.GetCharXY(position.x - 1, position.y))) {
		wallsCount++;
	}
	return (wallsCount < 3);
}

void GameManager::HandleFruitMovement()
{
	Point next_position;
	Point position = { fruit.GetPosition() };

	if (mode != "load") {
		next_position = fruit.CalcNextPosition(fruit.GetDirection());

		while (next_position.x > board.GetScreenSize().width - 1 || next_position.y > board.GetScreenSize().length - 1 ||
			next_position.x < 0 || next_position.y < 0) {
			fruit.HandleStay(isSilent);
			SetValidDirection(fruit);
			next_position = fruit.CalcNextPosition(fruit.GetDirection());
		}

		if (IsWall(this->board.GetCharXY(next_position.x, next_position.y))) {
			fruit.HandleStay(isSilent);
			SetValidDirection(fruit);
			next_position = fruit.CalcNextPosition(fruit.GetDirection());
		}
	}
	else {
		fruit.SetDirection(GetNextDirectionFromList(FileManager::FRUIT_LINE));
		next_position = fruit.CalcNextPosition(fruit.GetDirection());
	}

	if (board.GetCharXY(position.x, position.y) == BC)
	{
		fruit.Move(next_position.x, next_position.y, isSilent);
		if (!isSilent) {
			GoToXY(position.x, position.y);
			cout << BC;
		}
	}
	else if (board.GetCharXY(position.x, position.y) == WS) {
		fruit.Move(next_position.x, next_position.y, isSilent);
	}

	if (mode == "save") {
		fileManager.WriteDataToVector(ConvertDirectionToString(fruit.GetDirection()), FileManager::FRUIT_LINE);
	}
}

void GameManager::HandleEscape()
{
	system("CLS");
	cout << "Press ESC to unpause the game";
	while (1) {
		if (_kbhit() && _getch() == (char)eKeyBoardOptions::ESC) {
			system("CLS");
			break;
		}
	}
	LengthWidth lw = board.GetScreenSize();
	board.PrintBoard(lw.length, lw.width);
	int x = board.GetBannerX();
	int y = board.GetBannerY();
	for (int i = 0; i < board.GetNumOfGhosts(); i++) {
		ghosts_arr[i].Print(ghosts_arr->GetShape());
	}
	pacman.Print(pacman.GetShape());
	PrintHeadUpDisplay();
}

const void GameManager::PrintHeadUpDisplay()
{
	GoToXY(board.GetBannerX(), board.GetBannerY());
	cout << "Score: " << score << " Lives: " << lives << endl; //less than 20*3
}

bool GameManager::IsWall(char ch) const {
	if (ch == WALL)
		return true;
	else
		return false;
}

bool GameManager::IsDirection(char ch) const
{
	if (ch == (char)eDirection::DOWN || ch == (char)eDirection::UP || ch == (char)eDirection::RIGHT ||
		ch == (char)eDirection::LEFT || ch == (char)eDirection::STAY)
	{
		return true;
	}
	return false;
}

void GameManager::SetNewRandomDirection(Entity& entity)
{
	entity.SetDirection(RandomDirectionGenerator());
}

void GameManager::SetFruit() {
	Point position = { 0 };

	if (mode != "load") {
		do
		{
			position = RandomPositionGenerator(board.GetScreenSize().width, board.GetScreenSize().length);
		} while (board.GetCharXY(position.x, position.y) != BC && board.GetCharXY(position.x, position.y) != WS);
		this->fruit.SetX(position.x);
		this->fruit.SetY(position.y);
	}
	else {
		fruit.SetX(stoi(GetNextFruitDataFromList()));
		fruit.SetY(stoi(GetNextFruitDataFromList()));
		fruit.SetShape(GetNextFruitDataFromList()[0]);
	}

	if (mode == "save") {
		std::string fruitData;
		fruitData.append(std::to_string(position.x));
		fruitData.append(" ");
		fruitData.append(std::to_string(position.y));
		fruitData.append(" ");
		fruitData.push_back(fruit.GetShape());
		fileManager.WriteDataToVector(fruitData, FileManager::FRUIT_LINE);
	}
}