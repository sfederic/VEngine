#include "Log.h"
#include "Editor/Editor.h"

void Log(std::string msg)
{
	editor->Log(msg);
}

void Log(std::wstring msg)
{
	editor->Log(msg);
}
