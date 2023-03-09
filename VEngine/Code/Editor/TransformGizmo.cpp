#include "vpch.h"
#include "TransformGizmo.h"
#include "Render/Renderer.h"
#include "Core/Camera.h"
#include "Core/Input.h"
#include "Core/WorldEditor.h"
#include "Actors/Actor.h"
#include "Editor.h"
#include "Commands/CommandSystem.h"
#include "Commands/Command.h"
#include "Core/Core.h"
#include "Core/Log.h"
#include "Console.h"

TransformGizmo transformGizmo;

TransformGizmo::TransformGizmo()
{
    //Set translate as default startup transform operation
    currentSnapValues = translateSnapValues;

    ImGuizmo::AllowAxisFlip(false);
    ImGuizmo::SetGizmoSizeClipSpace(0.14f);
}

void TransformGizmo::Tick()
{
    if (!Core::isImGUIEnabled || Core::gameplayOn) return;

    //Set Imgui window
    ImGui::SetNextWindowSize(ImVec2(Renderer::GetViewportWidth(), Renderer::GetViewportHeight()));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Transform Gizmo", 0, ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoTitleBar);
    ImGuizmo::SetDrawlist();
    float windowWidth = (float)ImGui::GetWindowWidth();
    float windowHeight = (float)ImGui::GetWindowHeight();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

    //Setup camera matrices
    XMFLOAT4X4 view, proj, pickedObjectMatrix;
    XMStoreFloat4x4(&view, activeCamera->GetViewMatrix());
    XMStoreFloat4x4(&proj, activeCamera->GetProjectionMatrix());

    //Toggle and draw grid
    if (Input::GetKeyHeld(Keys::Shift))
    {
        if (Input::GetKeyDown(Keys::G))
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
    if (Console::bConsoleActive)
    {
        ImGui::End();
        return;
    }

    //Set transform operation
    if (!Input::GetMouseRightDown())
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
            editor->SetCurrentTransformMode("World");
        }
        else if (currentTransformMode == ImGuizmo::MODE::WORLD)
        {
            currentTransformMode = ImGuizmo::MODE::LOCAL;
            editor->SetCurrentTransformMode("Local");
        }
    }

    switch (WorldEditor::GetPickMode())
    {
        case WorldEditor::PickMode::Actor:
        {
            if (WorldEditor::GetPickedActor())
            {
                XMStoreFloat4x4(&pickedObjectMatrix, WorldEditor::GetPickedActor()->GetWorldMatrix());
            }
            break;
        }
        case WorldEditor::PickMode::Component:
        {
            if (WorldEditor::GetPickedComponent())
            {
                XMStoreFloat4x4(&pickedObjectMatrix, WorldEditor::GetPickedComponent()->GetWorldMatrix());
            }
            break;
        }
    }
    
    if (WorldEditor::GetPickedActor() == nullptr && WorldEditor::GetPickedComponent() == nullptr)
    {
        ImGui::End();
        return;
    }

    ImGuizmo::Manipulate(*view.m, *proj.m, currentTransformOperation, currentTransformMode,
        *pickedObjectMatrix.m, nullptr, currentSnapValues, bounds, boundsSnap);

    XMVECTOR scale, rot, trans;
    XMMatrixDecompose(&scale, &rot, &trans, XMLoadFloat4x4(&pickedObjectMatrix));

    if (CheckInUse())
    {
        switch (WorldEditor::GetPickMode())
        {
            case WorldEditor::PickMode::Actor:
            {
                if (WorldEditor::GetPickedActor())
                {
                    auto actor = WorldEditor::GetPickedActor();

                    actor->SetPosition(trans);
                    actor->SetScale(scale);
                    actor->SetRotation(rot);

                    editor->SetActorProps(actor);
                }
                break;
            }
            case WorldEditor::PickMode::Component:
            {
                if (WorldEditor::GetPickedComponent())
                {
                    auto component = WorldEditor::GetPickedComponent();

                    component->SetWorldPosition(trans);
                    component->SetWorldScale(scale);
                    component->SetWorldRotation(rot);

                    auto owner = World::GetActorByUID(component->GetOwnerUID());
                    if (owner)
                    {
                        editor->SetActorProps(owner);
                    }
                    else
                    {
                        Log("Component [%s] couldn't find Actor owner to set props from TransformGizmo");
                    }
                }
                break;
            }
        }

        mousePressAfterGizmoUse = true;
    }

    //When gizmo input ends, add change to CommandSystem
    if (mousePressAfterGizmoUse && Input::GetMouseLeftUp())
    {
        mousePressAfterGizmoUse = false;

        Actor* actor = WorldEditor::GetPickedActor();
        if (actor)
        {
            auto props = actor->GetProps();

            switch (currentTransformOperation)
            {
                case ImGuizmo::OPERATION::TRANSLATE:
                {
                    auto prop = props.GetProperty(" Position");
                    CommandSystem::Add(new Command<XMFLOAT3>(*prop));
                    break;
                }

                case ImGuizmo::OPERATION::SCALE:
                {
                    auto prop = props.GetProperty(" Scale");
                    CommandSystem::Add(new Command<XMFLOAT3>(*prop));
                    break;
                }

                case ImGuizmo::OPERATION::ROTATE:
                {
                    auto prop = props.GetProperty(" Rotation");
                    CommandSystem::Add(new Command<XMFLOAT4>(*prop));
                    break;
                }
            }
        }
    }

    //Toggle snap and scale controls
    if (Input::GetKeyHeld(Keys::Ctrl))
    {
        if (Input::GetKeyDown(Keys::O))
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

void TransformGizmo::SetLocalWorldTransformMode(const std::string transformMode)
{
    if (transformMode == "World")
    {
        currentTransformMode = ImGuizmo::MODE::WORLD;
    }
    else if (transformMode == "Local")
    {
        currentTransformMode = ImGuizmo::MODE::LOCAL;
    }
}
