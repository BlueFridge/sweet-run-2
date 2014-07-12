#include "LevelParser.h"
#include <cstdlib>
#include <sstream>

std::vector<std::string> LevelParser::split(std::string str, char delimiter)
{
	std::vector<std::string> outVect;
	std::stringstream ss;
	std::string tempStr;
	ss << str;
	while(std::getline(ss,tempStr,delimiter))
	{
		outVect.push_back(tempStr);
	}
	return outVect;
}

LevelParser::LevelParser()
{
	//Do nothing
}

LevelParser::LevelParser(std::string levelFile)
{
	this->levelFile = levelFile;
}

void LevelParser::setLevelFile(std::string levelFile)
{
	this->levelFile = levelFile;
}

void LevelParser::parseFile()
{
	std::ifstream ilstream(levelFile.c_str());
	std::string istring;
	while(std::getline(ilstream, istring))
	{
		std::vector<std::string> brokenStr = split(istring, '=');
		std::string command = brokenStr[0];
		std::string arg = brokenStr[1];
		if(command == "level_name")
			this->levelName = arg;
		else if(command == "stage_file")
			this->stageFile = arg;
		else if(command == "rows")
			this->rows = std::atoi(arg.c_str());
		else if(command == "row_coors_y")
		{
			std::vector<std::string> brokenCoors = split(arg, ',');
			std::vector<int> intCoors;
			for(int i = 0; i < brokenCoors.size(); i++)
			{
				intCoors.push_back(std::atoi(brokenCoors[i].c_str()));
			}
			this->yCoors = intCoors;
		}
		else if(command == "row_coors_x")
		{
			std::vector<std::string> brokenCoors = split(arg, ',');
			std::vector<std::pair<int, int> > xCoorsV;
			std::pair<int, int> intCoors;
			for(int i = 0; i < brokenCoors.size(); i++)
			{
				std::vector<std::string> coors = split(brokenCoors[i], '|');
				std::string a = coors[0];
				std::string b = coors[1];
				std::string start = a.substr(1, a.length());
				std::string end = b.substr(0, b.length()-1);
				intCoors = std::make_pair(std::atoi(start.c_str()), std::atoi(end.c_str()));
				xCoorsV.push_back(intCoors);
			}
			this->xCoors = xCoorsV;
		}
	}
	ilstream.close();
}

std::string LevelParser::getLevelName()
{
	return this->levelName;
}

std::string LevelParser::getStageFile()
{
	return this->stageFile;
}

int LevelParser::getRows()
{
	return this->rows;
}

std::vector<int> LevelParser::getYCoors()
{
	return this->yCoors;
}

std::vector<std::pair<int, int> > LevelParser::getXCoors()
{
	return this->xCoors;
}