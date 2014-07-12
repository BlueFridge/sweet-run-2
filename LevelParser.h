#ifndef LEVEL_P_H
#define LEVEL_P_H
#include <fstream>
#include <string>
#include <vector>
#include <utility>

class LevelParser
{
private:
	std::string levelName;
	std::string stageFile;
	std::string levelFile;
	int rows;
	std::vector<int> yCoors;
	std::vector<std::pair<int, int> > xCoors;
	std::vector<std::string> split(std::string str, char delimiter);
public:
	LevelParser();
	LevelParser(std::string levelFile);
	void setLevelFile(std::string levelFile);
	void parseFile();
	std::string getLevelName();
	std::string getStageFile();
	int getRows();
	std::vector<int> getYCoors();
	std::vector<std::pair<int,int> > getXCoors();
};

#endif