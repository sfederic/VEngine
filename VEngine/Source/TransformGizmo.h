#pragma once

#include "imgui/imgui.h"
#include "Imguizmo/ImGuizmo.h"

//Using ImGuizmo for transform, scale and rotation.
//Press W, E or R for Translation, Scale or Rotation.
class TransformGizmo
{
public:
	void Tick(ImGuiIO* io);

	ImGuizmo::OPERATION currentTransformOperation = ImGuizmo::TRANSLATE;

	float snap[3] = { 1.f, 1.f, 1.f };
	float bounds[6] = { -1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
	float boundsSnap[3] = { 0.5f, 0.5f, 0.5f };
};

extern TransformGizmo gTransformGizmo;
