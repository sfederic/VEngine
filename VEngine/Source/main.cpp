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

    //IMGUI setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    //Imgui has an .ini file to save previous ui positions and values.
    //Setting this to null removes this initial setup.
    io.IniFilename = nullptr;  

    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init((HWND)gEditorSystem->mainWindow);
    ImGui_ImplDX11_Init(gRenderSystem.device, gRenderSystem.context);

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

        gEditorSystem->ProcessEvents();
        gEditorSystem->Tick();

        gFileSystem.Tick();
        gUISystem.Tick();

        gTimerSystem.Tick(deltaTime);

        GetActiveCamera()->Tick(deltaTime);

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        gTransformGizmo.Tick();

        gDebugMenu.Tick(GetWorld(), deltaTime);

        ImGui::EndFrame();

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

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    //gUISystem.Cleanup();
    qApp->quit();

    return 0;
}
