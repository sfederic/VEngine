#include "DialogueStructures.h"
#include <fstream>

void Dialogue::LoadFromFile()
{
	if (filename.empty())
	{
		return;
	}

	std::string path = "Dialogues/" + filename;

	std::ifstream is;
	is.open(path, std::ios_base::in);

	while (!is.eof())
	{
		std::string lineText;
		std::string gotoText;
		std::string actorText;
		std::string intuitionText;
		std::string text;

		const int lineSize = 1024;
		char line[lineSize];

		//Get all text
		is.getline(line, lineSize);
		lineText.assign(line);
		if (lineText.empty())
		{
			break;
		}

		is.getline(line, lineSize);
		gotoText.assign(line);

		is.getline(line, lineSize);
		actorText.assign(line);

		is.getline(line, lineSize);
		intuitionText.assign(line);

		is.getline(line, lineSize);
		text.assign(line);


		//Get values and push them into the map
		int lineNumber = std::stoi(lineText);
		int gotoLine = -1;
		if (gotoText != "next")
		{
			gotoLine = std::stoi(gotoText);
		}

		DialogueData newData = {};
		newData.lineNumber = lineNumber;
		newData.gotoLine = gotoLine;
		newData.actorName = actorText;
		newData.intuitionName = intuitionText;
		newData.text = text;

		data[lineNumber] = newData;
	}

	is.close();
}
