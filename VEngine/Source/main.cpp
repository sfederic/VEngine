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
#include "ConsoleDock.h"
#include "RenderViewWidget.h"
#include "WorldDock.h"
#include "EditorSystem.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "TransformGizmo.h"
#include "ParticleSystem.h"
#include "Actors/TestActor.h"
#include "GlobalDefines.h"

#include "VWidget.h"
#include "Components/BoxTriggerComponent.h"
#include "DebugBox.h"
#include "DebugSphere.h"

int main(int argc, char* argv[])
{
    HR(CoInitialize(NULL)); //For the WIC texture functions from DXT

    gCoreSystem.Init();
    gEditorSystem->Init(argc, argv);
    
    gProfiler.Init();

    //FBX setup
    FBXImporter::Init();

    SetActiveCamera(&editorCamera);
    editorCamera.bEditorCamera = true;

    //Systems setup
    gCoreSystem.SetTimerFrequency();
    gRenderSystem.Init((HWND)gEditorSystem->mainWindow);
    gUISystem.Init();

    gDebugMenu.Init();
    gAudioSystem.Init();
    gWorldEditor.Init();

    

    //Test actor system stuff
    GetWorld()->AddActorSystem(&testActorSystem);

    Transform t;
    t.position = XMFLOAT3(-2.f, 0.f, 0.f);
    Actor* child = testActorSystem.SpawnActor(t);

    testActorSystem.actors[0]->SetPosition(2.f, 0.f, 0.f);
    testActorSystem.actors[0]->AddChild(child);


    //Test component stuff
    debugBox.Start();
    debugSphere.Start();


    //Test in-game UI widget stuff
    VWidget widget;
    gUISystem.AddWidget(&widget);

    //Qt late init
    gEditorSystem->PopulateWorldList();

    gRenderSystem.Flush();
    gRenderSystem.WaitForPreviousFrame();

    //MAIN LOOP
    while (gCoreSystem.bMainLoop)
    {
        const float deltaTime = gCoreSystem.deltaTime;

        gCoreSystem.StartTimer();

#ifndef EDITOR
        //Qt can have a bit of a fit here in strange widget cases handling Win32 msg's
        gCoreSystem.HandleMessages();
#endif

        gCoreSystem.Tick();

        gEditorSystem->ProcessEvents();
        gEditorSystem->Tick();

        gFileSystem.Tick();
        gUISystem.Tick();

        gTimerSystem.Tick(deltaTime);

        GetActiveCamera()->Tick(deltaTime);

        gRenderSystem.Tick();
        gRenderSystem.RenderSetup(deltaTime);

        gWorldEditor.Tick(nullptr);

        GetWorld()->TickAllActorSystems(deltaTime);

        gRenderSystem.Render(deltaTime);

        gRenderSystem.RenderEnd(deltaTime);

        //UI RENDERING
        gUISystem.d2dRenderTarget->BeginDraw();

        gUISystem.TickAllWidgets(deltaTime);

        gConsole.Tick();
        gConsole.DrawViewItems();
        gDebugMenu.Tick(GetWorld(), deltaTime);


        gUISystem.d2dRenderTarget->EndDraw();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        gRenderSystem.Flush();

        gRenderSystem.Present();

        gRenderSystem.WaitForPreviousFrame();

        gInputSystem.InputReset();
        gProfiler.Reset();

        gCoreSystem.EndTimer();
    }

    gDebugMenu.Cleanup();
    gUISystem.Cleanup();
    qApp->quit();

    return 0;
}
