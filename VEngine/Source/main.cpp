#include "..\EditorMainWindow.h"
#include "RenderSystem.h"
#include "CoreSystem.h"
#include "UISystem.h"
#include "Obj.h"
#include "Camera.h"
#include "Audio.h"
#include "AudioSystem.h"
#include "Input.h"
#include "Actor.h"
#include "ShaderFactory.h"
#include "DebugMenu.h"
#include "Raycast.h"
#include "World.h"
#include "FileSystem.h"
#include "Debug.h"
#include "FBXImporter.h"
#include "WorldEditor.h"
#include "TimerSystem.h"
#include "MathHelpers.h"
#include "Console.h"
#include "Profiler.h"
#include "TestActor.h"
#include "ConsoleDock.h"
#include "RenderViewWidget.h"
#include "WorldDock.h"

#include "EditorSystem.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

#include "TransformGizmo.h"

float roll = 0.f;

int main(int argc, char* argv[])
{
    HR(CoInitialize(NULL)); //For the WIC texture functions from DXT

    gEditorSystem->Init(argc, argv);
    
    gProfiler.Init();

    //FBX setup
    FBXImporter::Init();

    SetActiveCamera(&editorCamera);
    editorCamera.bEditorCamera = true;

    //Systems setup
    gCoreSystem.SetTimerFrequency();
    gRenderSystem.Init((HWND)gEditorSystem->mainWindow);
    gDebugMenu.Init();
    gAudioSystem.Init();
    //gUISystem.Init();
    gWorldEditor.Init();

    ActorSystem ac;
    ac.modelName = "material_test_cube.fbx";
    ac.shaderName = L"shaders.hlsl";
    ac.textureName = L"texture2.jpg";
    ac.name = L"Cubes";
    ac.CreateActors<Actor>(&gRenderSystem, 1);

    GetWorld()->AddActorSystem(ac);

    //Qt late init
    gEditorSystem->PopulateWorldList();

    gRenderSystem.Flush();
    gRenderSystem.WaitForPreviousFrame();

    //MAIN LOOP
    while (gCoreSystem.bMainLoop)
    {
        //TODO: fine for simple main loop for now, need to throw it into somewhere later.
        const float deltaTime = gCoreSystem.deltaTime;

        gCoreSystem.StartTimer();
        gCoreSystem.HandleMessages();
        gCoreSystem.Tick();

        gEditorSystem->ProcessEvents();
        gEditorSystem->Tick();

        gFileSystem.Tick();
        gUISystem.Tick();

        gTimerSystem.Tick(deltaTime);

        GetActiveCamera()->Tick(deltaTime);

        gDebugMenu.Tick(GetWorld(), deltaTime);

        gRenderSystem.Tick();
        gRenderSystem.RenderSetup(deltaTime);

        gWorldEditor.Tick(nullptr);

        GetWorld()->TickAllActorSystems(deltaTime);

        ImGui::Render();
        gRenderSystem.Render(deltaTime);
        gRenderSystem.RenderEnd(deltaTime);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        //UI RENDERING
        if (gUISystem.bAllUIActive)
        {
            /*gUISystem.d2dRenderTarget->BeginDraw();
            gConsole.Tick();
            gConsole.DrawViewItems();
            gDebugMenu.Tick(GetWorld(), deltaTime);
            gUISystem.RenderAllUIViews();
            gUISystem.d2dRenderTarget->EndDraw();*/
        }

        gRenderSystem.Flush();

        //PRESENT
        gRenderSystem.Present();

        gRenderSystem.WaitForPreviousFrame();

        gInputSystem.InputReset();
        gProfiler.Clean();

        gCoreSystem.EndTimer();
    }

    gDebugMenu.Cleanup();
    //gUISystem.Cleanup();
    qApp->quit();

    return 0;
}
