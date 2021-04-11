#pragma once

#include "imgui/imgui.h"
#include "Imguizmo/ImGuizmo.h"

//Using ImGuizmo for transform, scale and rotation. Press W, E or R for Translation, Scale or Rotation.
//NOTE: Keep in mind that ImGuizmo draws its gizmos inside an ImGui window, and not directly onto the viewport.
class TransformGizmo
{
public:
	void Tick();

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
};

extern TransformGizmo gTransformGizmo;
