#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace nc
{
	class File
	{
	public:
		int static LoadHighScore(const std::string& filename);
		void static SaveHighScore(const std::string& filename, int newHighScore);
	};
}
