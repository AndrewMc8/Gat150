#include "File.h"

int nc::File::LoadHighScore(const std::string& filename)
{
	int highscore = -1;
	std::fstream file;
	file.open(filename, std::ios::in);

	if (file.is_open())
	{
		file >> highscore;
		file.close();
	}

	/*if (file)
	{
		char ch;
		std::string s = "";

		while (1)
		{
			file >> ch;
			if (file.eof()) break;
			s += ch;
		}

		try { highscore = std::stoi(s); }
		catch (...) { std::cerr << "WRONG!\n"; }
	}
	else
	{
		std::cout << "File not found" << std::endl;
	}*/
	return highscore;
}

void nc::File::SaveHighScore(const std::string& filename, int newHighScore)
{
	std::fstream file;
	file.open(filename, std::ios::out);
	if (file.is_open())
	{
		file << newHighScore;
		file.close();
	}
}
