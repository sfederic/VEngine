export module Render.DebugMeshManager;

import <string>;

export class MeshComponent;

export class DebugMeshManager
{
public:
	static void CreateDebugMeshes()
	{
		AddDebugMesh("DebugBox", "cube.vmesh");
		AddDebugMesh("DebugCamera", "camera.vmesh");
		AddDebugMesh("DebugCapsule", "capsule.vmesh");
		AddDebugMesh("DebugCone", "small_cone.vmesh");
		AddDebugMesh("DebugIcoSphere", "small_ico_sphere.vmesh");
		AddDebugMesh("DebugSphere", "ico_sphere.vmesh");
	}

	static void DestroyDebugMeshes();

	static MeshComponent* GetDebugMesh(const std::string& name);

private:
	static void AddDebugMesh(const std::string& name, const std::string& meshFile);
};
