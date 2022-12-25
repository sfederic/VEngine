#pragma once

//Just the mesh for any camera components in the scene
struct DebugCamera
{
	class MeshComponent* mesh = nullptr;

	DebugCamera();
};
