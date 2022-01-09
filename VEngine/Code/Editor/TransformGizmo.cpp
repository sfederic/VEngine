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

TransformGizmo::TransformGizmo()
{
    //Set translate as default startup transform operation
    currentSnapValues = translateSnapValues;
}

void TransformGizmo::Tick()
{
    if (!Core::isImGUIEnabled || Core::gameplayOn) return;

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
    XMStoreFloat4x4(&proj, activeCamera->GetProjectionMatrix());


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
                currentSnapValues = translateSnapValues;
            }
            else if (Input::GetKeyDown(Keys::E))
            {
                currentTransformOperation = ImGuizmo::SCALE;
                currentSnapValues = scaleSnapValues;
            }
            else if (Input::GetKeyDown(Keys::R))
            {
                currentTransformOperation = ImGuizmo::ROTATE;
                currentSnapValues = rotationSnapValues;
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
        if (Input::GetAsyncKey(Keys::Ctrl))
        {
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
    }

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

void TransformGizmo::SetTranslateSnapValue(float value)
{
    translateSnapValues[0] = value;
    translateSnapValues[1] = value;
    translateSnapValues[2] = value;
}

void TransformGizmo::SetScaleSnapValue(float value)
{
    scaleSnapValues[0] = value;
    scaleSnapValues[1] = value;
    scaleSnapValues[2] = value;
}

void TransformGizmo::SetRotationSnapValue(float value)
{
    rotationSnapValues[0] = value;
    rotationSnapValues[1] = value;
    rotationSnapValues[2] = value;
}
