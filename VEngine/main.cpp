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

    //FBX setup
    FBXImporter::Init();

    editorCamera.bEditorCamera = true;

    //Systems setup
    gCoreSystem.SetTimerFrequency();
    gRenderSystem->Init((HWND)editorMainWindow->renderViewWidget->winId());
    gAudioSystem.Init();
    gUISystem.Init();
    gWorldEditor.Init();

    //TODO: This is causing a crash in Release. Is it because it's after the init code?
    //ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

    ActorSystem cubes;
    cubes.modelName = "cube.fbx";
    cubes.name = L"TestCubes";
    cubes.CreateActors(gRenderSystem, 3);

    ActorSystem player = {};
    player.name = L"Player0";
    player.modelName = "monkey.fbx";
    player.CreateActors(gRenderSystem, 1);


    World* world = GetWorld();
    world->actorSystems.push_back(&cubes);
    world->AddActorSystem(player);
    world->actorSystems.push_back(&gWorldEditor.xAxis);
    world->actorSystems.push_back(&gWorldEditor.yAxis);
    world->actorSystems.push_back(&gWorldEditor.zAxis);

    editorMainWindow->worldDock->PopulateWorldList();

    gRenderSystem->Flush();

    bool bGameOn = false;
    bool bMoveLeft = true;

    //MAIN LOOP
    while (gCoreSystem.bMainLoop)
    {
        const float deltaTime = gCoreSystem.deltaTime;

        gCoreSystem.StartTimer();
        gCoreSystem.HandleMessages();

        qApplication.processEvents();
        editorMainWindow->Tick();

        gFileSystem.Tick();
        gUISystem.Tick();

        //TODO: move to CoreSystem/UISystem.
        QPoint p = editorMainWindow->renderViewWidget->mapFromGlobal(QCursor::pos());
        gUISystem.mousePos.x = p.x();
        gUISystem.mousePos.y = p.y();

        gTimerSystem.Tick(deltaTime);

        if (gInputSystem.GetKeyDownState('P'))
        {
            bGameOn = !bGameOn;
        }

        //if (bGameOn)
        {
            //SetActiveCamera(&playerCamera);
            //GetActiveCamera()->AttachTo(player.GetActor(0));
            //playerCamera.Tick(deltaTime);

            Actor* playerActor = player.GetActor(0);
            if (gInputSystem.GetKeyDownState(VK_LEFT))
            {
                bMoveLeft = false;
                
            }

            if (bMoveLeft)
            {
                XMVECTOR endPos = DirectX::XMVectorSet(2.f, 0.f, 0.f, 1.f);
                playerActor->SetPosition(XMVectorLerp(playerActor->GetPositionVector(), endPos, deltaTime * 5.0f));
            }
            else if (!bMoveLeft)
            {
                XMVECTOR endPos = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
                playerActor->SetPosition(XMVectorLerp(playerActor->GetPositionVector(), endPos, deltaTime * 5.0f));
            }

            if (gInputSystem.GetKeyDownState(VK_RIGHT))
            {
                bMoveLeft = true;
                
            }
        }
        //else
        {
            SetActiveCamera(&editorCamera);
            editorCamera.Tick(deltaTime);
        }


        gRenderSystem->Tick();
        gRenderSystem->RenderSetup(deltaTime);

        gWorldEditor.Tick(nullptr, editorMainWindow);

        gRenderSystem->Render(deltaTime);
        gRenderSystem->RenderEnd(deltaTime);

        //UI RENDERING
        if (gUISystem.bAllUIActive)
        {
            gUISystem.d2dRenderTarget->BeginDraw();
            gConsole.Tick();
            gConsole.DrawViewItems();
            //debugMenu.Tick(GetWorld(), deltaTime);
            //gUISystem.RenderAllUIViews();
            gUISystem.d2dRenderTarget->EndDraw();
        }

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
