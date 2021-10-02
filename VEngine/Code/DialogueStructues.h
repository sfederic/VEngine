#pragma once
#include <map>
#include <string>

struct DialogueData
{
	int lineNumber = 0;
	int gotoLine = 0;
	std::string actorName;
	std::string intuitionName;
	std::string text;
};

struct Dialogue
{
	std::map<int, DialogueData> data;
};
