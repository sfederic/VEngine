#include "TransformGizmo.h"
#include <DirectXMath.h>
#include "Render/Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "WorldEditor.h"
#include "Actors/Actor.h"
#include "Editor.h"
#include "Commands/CommandSystem.h"
#include "Commands/Command.h"
#include "Core.h"
#include "Console.h"

TransformGizmo transformGizmo;

void TransformGizmo::Tick()
{
    //Set Imgui window
    ImGui::SetNextWindowSize(ImVec2(renderer.GetViewportWidth(), renderer.GetViewportHeight()));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Transform Gizmo", 0, ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoTitleBar);
    ImGuizmo::SetDrawlist();
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);


    //Setup camera matrices
    XMFLOAT4X4 view, proj, actorMatrix;
    XMStoreFloat4x4(&view, activeCamera->GetViewMatrix());
    XMStoreFloat4x4(&proj, activeCamera->proj);


    //Toggle and draw grid
    if (Input::GetAsyncKey(Keys::ShiftLeft))
    {
        if (Input::GetKeyUp(Keys::G))
        {
            bGridToggle = !bGridToggle;
        }
    }

    if (bGridToggle && !Core::gameplayOn)
    {
        XMFLOAT4X4 identity;
        XMStoreFloat4x4(&identity, XMMatrixIdentity());
        ImGuizmo::DrawGrid(&view.m[0][0], &proj.m[0][0], &identity.m[0][0], 20.f);
    }


    //Exit early if console is active
    if (console.bConsoleActive)
    {
        ImGui::End();
        return;
    }


    if (worldEditor.pickedActor)
    {
        //Set transform operation
        if (!Input::GetAsyncKey(Keys::MouseRight))
        {
            if (Input::GetKeyDown(Keys::W))
            {
                currentTransformOperation = ImGuizmo::TRANSLATE;
                currentSnapValues[0] = translateSnapValues[0];
                currentSnapValues[1] = translateSnapValues[1];
                currentSnapValues[2] = translateSnapValues[2];
            }
            else if (Input::GetKeyDown(Keys::E))
            {
                currentTransformOperation = ImGuizmo::SCALE;
                currentSnapValues[0] = scaleSnapValues[0];
                currentSnapValues[1] = scaleSnapValues[1];
                currentSnapValues[2] = scaleSnapValues[2];
            }
            else if (Input::GetKeyDown(Keys::R))
            {
                currentTransformOperation = ImGuizmo::ROTATE;
                currentSnapValues[0] = rotationSnapValues[0];
                currentSnapValues[1] = rotationSnapValues[1];
                currentSnapValues[2] = rotationSnapValues[2];
            }
        }

        //Set Transform mode between world and local for gizmo
        if (Input::GetKeyUp(Keys::Space))
        {
            if (currentTransformMode == ImGuizmo::MODE::LOCAL)
            {
                currentTransformMode = ImGuizmo::MODE::WORLD;
            }
            else if (currentTransformMode == ImGuizmo::MODE::WORLD)
            {
                currentTransformMode = ImGuizmo::MODE::LOCAL;
            }
        }


        XMStoreFloat4x4(&actorMatrix, worldEditor.pickedActor->GetWorldMatrix());

        //Render gizmos and set component values back to actor
        ImGuizmo::Manipulate(*view.m, *proj.m, currentTransformOperation, currentTransformMode,
            *actorMatrix.m, nullptr, currentSnapValues, bounds, boundsSnap);

        if (CheckInUse())
        {
            Actor* actor = worldEditor.pickedActor;

            XMVECTOR scale, rot, trans;
            XMMatrixDecompose(&scale, &rot, &trans, XMLoadFloat4x4(&actorMatrix));

            actor->SetPosition(trans);
            actor->SetScale(scale);
            actor->SetRotation(rot);

            editor->SetActorProps(actor);

            mousePressAfterGizmoUse = true;
        }

        //When gizmo input ends, add change to CommandSystem
        if (mousePressAfterGizmoUse && Input::GetMouseLeftUp())
        {
            mousePressAfterGizmoUse = false;

            Actor* actor = worldEditor.pickedActor;
            commandSystem.Add(new Command<Transform>(&actor->rootComponent->transform));
        }

        //Toggle snap and scale controls
        if (Input::GetKeyUp(Keys::O))
        {
            bBoundsToggle = !bBoundsToggle;
            if (!bBoundsToggle)
            {
                memset(bounds, 0.f, sizeof(float) * 6);
                memset(boundsSnap, 0.f, sizeof(float) * 3);
            }
            else if (bBoundsToggle)
            {
                bounds[0] = -1.f;
                bounds[1] = -1.f;
                bounds[2] = -1.f;
                bounds[3] = 1.f;
                bounds[4] = 1.f;
                bounds[5] = 1.f;
                memset(boundsSnap, 0.5f, sizeof(float) * 3);
            }
        }
    }

    //TODO: View Manipulator (Gives a little Autodesk-esque widget in the corner, but I can't figure it out. Camera class is a bit wonky)
    /*ImGuiIO& io = ImGui::GetIO();
    float viewManipulateRight = io.DisplaySize.x;
    float viewManipulateTop = 0;
    viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
    viewManipulateTop = ImGui::GetWindowPos().y;

    const float camDistance = 8.f;
    ImGuizmo::ViewManipulate(&view.m[0][0], camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);*/

    ImGui::End();
}

bool TransformGizmo::CheckInUse()
{
    return ImGuizmo::IsUsing();
}

bool TransformGizmo::CheckMouseOver()
{
    return ImGuizmo::IsOver();
}
