#include "NativeEditorSystem.h"
#include "CoreSystem.h"

void NativeEditorSystem::Init(int argc, char** argv)
{
	gCoreSystem.windowWidth = 800;
	gCoreSystem.windowHeight = 600;
	gCoreSystem.SetupWindow(GetModuleHandle(NULL), SW_SHOW);
	mainWindow = gCoreSystem.mainWindow;
}
