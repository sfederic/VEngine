#pragma once
#include <map>
#include <string>

struct DialogueData
{
	int lineNumber = 0;
	int gotoLine = -1; //-1 denotes that the dialogue continues linearly
	std::string actorName;
	std::string intuitionName;
	std::string text;
};

//TODO: think about making a widget for Dialogue struct like how Texture/Mesh/Shader have
struct Dialogue
{
	//The filename of the .dialog file to read from
	std::string filename;

	std::map<int, DialogueData> data;

	void LoadFromFile();
};
