#include "vpch.h"
#include "DialogueStructures.h"
#include <fstream>
#include <filesystem>
#include "Core/Log.h"
#include "Asset/AssetPaths.h"

void Dialogue::LoadFromFile()
{
	if (filename.empty())
	{
		return;
	}

	if (!std::filesystem::exists(AssetBaseFolders::dialogue + filename))
	{
		Log("Dialogue file [%s] not found.", filename.c_str());
		return;
	}

	std::string path = AssetBaseFolders::dialogue + filename;

	std::wifstream is;
	is.open(path, std::ios_base::in);

	while (!is.eof())
	{
		std::wstring lineText;
		std::wstring gotoText;
		std::wstring actorText;
		std::wstring conditionText;
		std::wstring conditionArgText;
		std::wstring text;

		const int lineSize = 1024;
		wchar_t line[lineSize];

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
		conditionText.assign(line);

		is.getline(line, lineSize);
		conditionArgText.assign(line);

		is.getline(line, lineSize);
		text.assign(line);


		//Get values and push them into the map
		int lineNumber = std::stoi(lineText);
		int gotoLine = -1;
		if (gotoText != L"next")
		{
			gotoLine = std::stoi(gotoText);
		}

		DialogueData newData = {};
		newData.lineNumber = lineNumber;
		newData.gotoLine = gotoLine;
		newData.actorName = actorText;
		newData.conditionName = conditionText;
		newData.conditionArg = conditionArgText;
		newData.text = text;

		data[lineNumber] = newData;
	}

	is.close();
}
