#pragma once
#include <map>
#include <string>

struct DialogueData
{
	int lineNumber = 0;
	int gotoLine = -1; //-1 denotes that the dialogue continues linearly
	std::wstring actorName;
	std::wstring conditionName;
	std::wstring conditionArg;
	std::wstring text;
};

struct Dialogue
{
	//The filename of the .dialog file to read from
	std::string filename;

	std::map<int, DialogueData> data;

	void LoadFromFile();
};
