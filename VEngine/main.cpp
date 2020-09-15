#include "EditorMainWindow.h"
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

int main(int argc, char *argv[])
{
    //Qt setup
    QApplication qApplication(argc, argv);
    EditorMainWindow* editorMainWindow = new EditorMainWindow();
    editorMainWindow->setWindowState(Qt::WindowMaximized);
    editorMainWindow->show();
    editorMainWindow->setWindowTitle("Engine");

    FBXImporter::Init();

    gCoreSystem.SetTimerFrequency();
    gRenderSystem->Init((HWND)editorMainWindow->renderViewWidget->winId());
    gAudioSystem.Init();
    gUISystem.Init();
    gWorldEditor.Init();

    //TODO: This is causing a crash in Release. Is it because it's after the init code?
    //ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

    ActorSystem cubes;
    cubes.modelName = "cube.fbx";
    cubes.CreateActors(gRenderSystem, 1);

    World* world = GetWorld();
    world->actorSystems.push_back(&cubes);
    world->actorSystems.push_back(&gWorldEditor.xAxis);
    world->actorSystems.push_back(&gWorldEditor.yAxis);
    world->actorSystems.push_back(&gWorldEditor.zAxis);

    gRenderSystem->Flush();

    //MAIN LOOP
    while (gCoreSystem.bMainLoop)
    {
        const float deltaTime = gCoreSystem.deltaTime;

        qApplication.processEvents();

        gCoreSystem.StartTimer();
        gCoreSystem.HandleMessages();

        gFileSystem.Tick();
        gUISystem.Tick();

        QPoint p = editorMainWindow->renderViewWidget->mapFromGlobal(QCursor::pos());
        gUISystem.mousePos.x = p.x();
        gUISystem.mousePos.y = p.y();

        editorCamera.Tick(deltaTime);
        gTimerSystem.Tick(deltaTime);

        //test ortho work
        if (gInputSystem.GetKeyDownState('P'))
        {
            gRenderSystem->matrices.proj = XMMatrixOrthographicOffCenterLH(-5.f, 5.f, -5.f, 5.f, -50.f, 1000.f);
        }

        gRenderSystem->Tick();
        gRenderSystem->RenderSetup(deltaTime);

        gWorldEditor.Tick(nullptr, editorMainWindow);

        gRenderSystem->Render(deltaTime);
        gRenderSystem->RenderEnd(deltaTime);

        //UI RENDERING
        /*if (gUISystem.bAllUIActive)
        {
            gUISystem.d2dRenderTarget->BeginDraw();
            gConsole.Tick();
            gConsole.DrawViewItems();
            //debugMenu.Tick(GetWorld(), deltaTime);
            gUISystem.RenderAllUIViews();
            gUISystem.d2dRenderTarget->EndDraw();
        }*/

        gRenderSystem->Flush();

        //PRESENT
        gRenderSystem->Present();

        gInputSystem.InputReset();
        gCoreSystem.EndTimer();
    }

    gUISystem.Cleanup();
    qApp->quit();

    return 0;
}


