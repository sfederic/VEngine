#include "TransformGizmo.h"
#include "WorldEditor.h"
#include "Camera.h"
#include "CoreSystem.h"
#include "Input.h"

TransformGizmo gTransformGizmo;

void TransformGizmo::Tick(ImGuiIO* io)
{
    if (gWorldEditor.pickedActor)
    {
        //Setup camera and actor matrices
        XMFLOAT4X4 view, proj, actorMatrix;
        XMStoreFloat4x4(&view, GetActiveCamera()->view);
        XMStoreFloat4x4(&proj, GetActiveCamera()->proj);
        XMStoreFloat4x4(&actorMatrix, gWorldEditor.pickedActor->GetTransformationMatrix());

        //Set Imgui window
        ImGui::SetNextWindowSize(ImVec2(gCoreSystem.windowWidth, gCoreSystem.windowHeight));
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Transform Gizmo", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
        ImGuizmo::SetDrawlist();
        float windowWidth = (float)ImGui::GetWindowWidth();
        float windowHeight = (float)ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

        //Set transform operation
        if (gInputSystem.GetKeyDownState('W'))
        {
            currentTransformOperation = ImGuizmo::TRANSLATE;
        }      
        else if (gInputSystem.GetKeyDownState('E'))
        {
            currentTransformOperation = ImGuizmo::SCALE;
        }
        else if (gInputSystem.GetKeyDownState('R'))
        {
            currentTransformOperation = ImGuizmo::ROTATE;
        }

        //Render gizmos and set component values back to actor
        ImGuizmo::Manipulate(&view.m[0][0], &proj.m[0][0], currentTransformOperation, ImGuizmo::MODE::LOCAL, &actorMatrix.m[0][0],
            nullptr, snap, bounds, boundsSnap);
        Actor* actor = gWorldEditor.pickedActor;
        XMFLOAT3 axis;
        ImGuizmo::DecomposeMatrixToComponents(&actorMatrix.m[0][0],
            &actor->transform.position.x,
            &axis.x,
            &actor->transform.scale.x);
        actor->SetRotation(axis);

        XMFLOAT4X4 identity;
        XMStoreFloat4x4(&identity, XMMatrixIdentity());
        ImGuizmo::DrawGrid(&view.m[0][0], &proj.m[0][0], &identity.m[0][0], 100.f);

        //TODO: View Manipulator (Gives a little Autodesk-esque widget in the corner, but I can't figure it out)
        float viewManipulateRight = io->DisplaySize.x;
        float viewManipulateTop = 0;
        viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
        viewManipulateTop = ImGui::GetWindowPos().y;
        ImGuizmo::ViewManipulate(&view.m[0][0], 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
        GetActiveCamera()->view = XMLoadFloat4x4(&view);

        ImGui::End();
    }
}