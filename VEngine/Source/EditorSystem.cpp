#include "EditorSystem.h"
#include "ToolkitEditorSystem.h"
#include "NativeEditorSystem.h"
#include "GlobalDefines.h"

#ifdef EDITOR
IEditorSystem* gEditorSystem = new ToolkitEditorSystem();
#else
IEditorSystem* gEditorSystem = new NativeEditorSystem();
#endif
