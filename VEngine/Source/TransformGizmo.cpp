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

        //Toggle and draw grid
        if (gInputSystem.GetKeyUpState('G'))
        {
            bGridToggle = !bGridToggle;
        }
        if (bGridToggle)
        {
            XMFLOAT4X4 identity;
            XMStoreFloat4x4(&identity, XMMatrixIdentity());
            ImGuizmo::DrawGrid(&view.m[0][0], &proj.m[0][0], &identity.m[0][0], 100.f);
        }

        //TODO: View Manipulator (Gives a little Autodesk-esque widget in the corner, but I can't figure it out. Camera class is a bit wonky)
        /*float viewManipulateRight = io->DisplaySize.x;
        float viewManipulateTop = 0;
        viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
        viewManipulateTop = ImGui::GetWindowPos().y;
        ImGuizmo::ViewManipulate(&view.m[0][0], 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
        GetActiveCamera()->view = XMLoadFloat4x4(&view);
        GetActiveCamera()->forward = XMVectorSet(view._31, view._32, view._33, 0.f);
        GetActiveCamera()->right = XMVectorSet(view._11, view._12, view._13, 0.f);
        GetActiveCamera()->up = XMVectorSet(view._21, view._22, view._23, 0.f);*/

        //Toggle snap and scale controls
        if (gInputSystem.GetKeyUpState('O'))
        {
            bBoundsToggle = !bBoundsToggle;
            if (bBoundsToggle)
            {
                memset(bounds, 0.f, sizeof(float) * 6);
                memset(boundsSnap, 0.f, sizeof(float) * 3);
            }
            else if (!bBoundsToggle)
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

        ImGui::End();
    }
}