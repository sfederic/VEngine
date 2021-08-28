#pragma once
#include "imgui.h"
#include "ImGuizmo.h"

//REF: Blog about building custom imgui gizmos.
//https://maxliani.wordpress.com/2021/06/26/offline-to-realtime-gesture-graphics/

//NOTE: There was an issue with ImGuizmo where the rotation gizmo would raycast in the opposite direction
//(with LH matrices?) and only hit the back of the gizmo. The fix was in ImGuizmo.cpp at line 1777.
//https://github.com/CedricGuillemet/ImGuizmo/issues/144

//Using ImGuizmo for transform, scale and rotation. Press W, E or R for Translation, Scale or Rotation.
//NOTE: Keep in mind that ImGuizmo draws its gizmos inside an ImGui window, and not directly onto the viewport.
struct TransformGizmo
{
	ImGuizmo::OPERATION currentTransformOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE currentTransformMode = ImGuizmo::WORLD;

	float currentSnapValues[3] = { 1.f, 1.f, 1.f };
	float translateSnapValues[3] = { 1.f, 1.f, 1.f };
	float rotationSnapValues[3] = { 10.f, 10.f, 10.f };
	float scaleSnapValues[3] = { 0.5f, 0.5f, 0.5f };

	float bounds[6] = { -1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
	float boundsSnap[3] = { 0.5f, 0.5f, 0.5f };

	bool bBoundsToggle = true;
	bool bGridToggle = true;

	bool mousePressAfterGizmoUse = false;

	void Tick(); //Needs to be called in DebugMenu::Tick() to work with ImGui calls
	bool CheckInUse();
	bool CheckMouseOver();
};

extern TransformGizmo transformGizmo;
