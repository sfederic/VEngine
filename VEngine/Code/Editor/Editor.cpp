#include "vpch.h"
#include "Editor.h"
#include "QtEditor.h"
//#include "Win32Editor.h"

static QtEditor gEditor;

IEditor& Editor::Get()
{
	return gEditor;
}
