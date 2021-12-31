#pragma once
#include "imgui.h"
#include "ImGuizmo.h"

//REF: Blog about building custom imgui gizmos.
//https://maxliani.wordpress.com/2021/06/26/offline-to-realtime-gesture-graphics/

//REF: OurMachinery guys also have a 3 part blog about gizmo related stuff
//https://ourmachinery.com/post/borderland-between-rendering-and-editor-part-1/

//NOTE: There was an issue with ImGuizmo where the rotation gizmo would raycast in the opposite direction
//(with LH matrices?) and only hit the back of the gizmo. The fix was in ImGuizmo.cpp at line 1777.
//https://github.com/CedricGuillemet/ImGuizmo/issues/144

//Using ImGuizmo for transform, scale and rotation. Press W, E or R for Translation, Scale or Rotation.
//Keep in mind that ImGuizmo draws its gizmos inside an ImGui window, and not directly onto the viewport.
//Also, Imguizmo's grid renders over the top of actors in the world, but trying to fix it just brings
//up all sorts of mouse input logic that isn't worth having.
struct TransformGizmo
{
	ImGuizmo::OPERATION currentTransformOperation = ImGuizmo::TRANSLATE;
	ImGuizmo::MODE currentTransformMode = ImGuizmo::WORLD;

	//Snap values are currently uniform across each axis, Imguizmo can handle seperate axis snap values though.
	float* currentSnapValues = nullptr;
	float translateSnapValues[3] = { 1.f, 1.f, 1.f };
	float rotationSnapValues[3] = { 90.f, 90.f, 90.f };
	float scaleSnapValues[3] = { 0.5f, 0.5f, 0.5f };

	float bounds[6] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	float boundsSnap[3] = { 0.5f, 0.5f, 0.5f };

	bool bBoundsToggle = true;
	bool bGridToggle = false;

	bool mousePressAfterGizmoUse = false;

	TransformGizmo();

	//Needs to be called in DebugMenu::Tick() to work with ImGui calls
	void Tick();

	bool CheckInUse();
	bool CheckMouseOver();
	void SetTranslateSnapValue(float value);
	void SetScaleSnapValue(float value);
	void SetRotationSnapValue(float value);
};

extern TransformGizmo transformGizmo;
