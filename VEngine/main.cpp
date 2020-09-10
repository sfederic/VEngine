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
#include <thread>
#include "ShaderFactory.h"
#include <omp.h>
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
    QApplication a(argc, argv);
    EditorMainWindow* app = new EditorMainWindow();
    app->setWindowState(Qt::WindowMaximized);

    app->show();

    app->setWindowTitle("Testing");

    FBXImporter::Init();

    //gCoreSystem.SetupWindow(GetModuleHandle(NULL), 1);
    gCoreSystem.windowWidth = app->width();
    gCoreSystem.windowHeight = app->height();
    gCoreSystem.SetTimerFrequency();
    gRenderSystem->Init((HWND)app->mainWidget.winId());
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

        a.processEvents();

        //QT ticks testing
        //propWidget->Tick();

        gCoreSystem.StartTimer();
        gCoreSystem.HandleMessages();

        gFileSystem.Tick();
        gUISystem.Tick();

        QPoint p = app->mainWidget.mapFromGlobal(QCursor::pos());
        gUISystem.mousePos.x = p.x();
        gUISystem.mousePos.y = p.y();

        editorCamera.Tick(deltaTime);
        gTimerSystem.Tick(deltaTime);

        //test ortho work
        if (gInputSystem.GetKeyDownState('P'))
        {
            gRenderSystem->matrices.proj = XMMatrixOrthographicOffCenterLH(-5.f, 5.f, -5.f, 5.f, -50.f, 1000.f);
        }

        //ACTOR RENDERING
        gRenderSystem->Tick();
        gRenderSystem->RenderSetup(deltaTime);

        gWorldEditor.Tick(nullptr);

        gRenderSystem->Render(deltaTime);
        gRenderSystem->RenderEnd(deltaTime);

        

        //UI RENDERING
        if (gUISystem.bAllUIActive)
        {
            gUISystem.d2dRenderTarget->BeginDraw();
            gConsole.Tick();
            gConsole.DrawViewItems();
            //debugMenu.Tick(GetWorld(), deltaTime);
            gUISystem.RenderAllUIViews();
            gUISystem.d2dRenderTarget->EndDraw();
        }

        //PRESENT
        gRenderSystem->Present();

        gInputSystem.InputReset();

        gCoreSystem.EndTimer();
    }

    gUISystem.Cleanup();
    qApp->quit();

    return 0;
}


