#include "files_utils.h"

using namespace std;

InputParser::InputParser(int& argc, char** argv)
{
	for (int i = 0; i < argc; ++i)
		this->tokens.push_back(std::string(argv[i]));
}

bool InputParser::IsOptionExistInCmd(const std::string& option)
{
	return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
}


std::vector<std::string> FileManager::GetStepsVector()
{
	return this->stepsString;
}

void FileManager::WriteDataToVector(std::string step, int entity_line) {
	step.append(" ");
	this->stepsString[entity_line].append(step);
}

void FileManager::WriteStepsToFile(std::string& screen_name)
{
	string file_name = this->EditFileName(screen_name, "steps");
	ofstream file(file_name);
	if (file.is_open())
	{
		for (int i = 0; i < stepsString.size(); i++)
		{
			file << this->stepsString[i];
			file << '\n';
		}
		file.close();
	}
	else cout << "Unable to open file";
}

void FileManager::WriteResultToFile(std::vector<std::string> result, std::string& screen_name) //vector of screen names[curr_screen]
{
	string file_name = this->EditFileName(screen_name, "result");
	ofstream file(file_name);
	if (file.is_open())
	{
		file << result[0] << endl;
		file << result[1] << endl;
		file.close();
	}
	else cout << "Unable to open file";
}

std::string FileManager::EditFileName(std::string& screen_name, std::string end) //take the screens names, remove ".screen", instead add ".steps"
{
	string temp = screen_name;
	int loc_after_period = temp.find('.') + 1;
	temp.replace(temp.begin() + loc_after_period, temp.end(), end);
	return temp;
}

//"UP DOWN LEFT LEFT RIGHT"
//"UP DOWN LEFT LEFT RIGHT"
//"UP DOWN LEFT LEFT RIGHT"
//"UP DOWN LEFT LEFT RIGHT"
//"UP DOWN LEFT LEFT RIGHT"
//"(x,y) UP DOWN LEFT LEFT RIGHT (x,y) UP DOWN LEFT LEFT RIGHT"


std::vector<std::list<std::string>> FileManager::ReadStepsFromFileToVector(std::string& file_name)
{
	int curr_space = 0;
	std::string line, token;
	std::vector<std::list<std::string>> vec;
	ifstream file(file_name);
	if (file.is_open())
	{
		for (int i = 0; i < NUM_MAX_ENTITIES; i++)
		{
			std::list<std::string> list;
			getline(file, line);
			if (line != "\n") {
				while (curr_space >= 0)
				{
					curr_space = line.find(' ');
					token = line.substr(0, curr_space);
					line = line.substr(curr_space + 1);
					list.push_back(token);
				}
				vec.push_back(list);
				curr_space = 0;
			}
		}
		file.close();
		return vec;
	}
	else
		cout << "File can't be opened"; // HANDLE!!
}

std::vector<std::list<int>> FileManager::ReadResultFromFileToVector(std::string& file_name)
{
	vector<list<int>> result(2);
	ifstream file(file_name);
	string str, token;
	int curr_space = 0;
	if (file.is_open())
	{
		for (int i = 0; i < result.size(); i++)
		{
			getline(file, str);
			if (str != "\n") {
				while (curr_space >= 0)
				{
					curr_space = str.find(' ');
					token = str.substr(0, curr_space);
					str = str.substr(curr_space + 1);
					if (token != "") {
						result[i].push_back(stoi(token));
					}
				}
				curr_space = 0;
			}
		}
		file.close();
		return result;
	}
	else
		cout << "File can't be opened"; // HANDLE!!
}
