#include "vpch.h"
#include "DebugMeshManager.h"
#include "Components/MeshComponent.h"
#include <memory>
#include <unordered_map>

std::unordered_map<std::string, std::unique_ptr<MeshComponent>> debugMeshes;

void DebugMeshManager::DestroyDebugMeshes()
{
	debugMeshes.clear();
}

MeshComponent* DebugMeshManager::GetDebugMesh(const std::string& name)
{
	return debugMeshes.find(name)->second.get();
}

void DebugMeshManager::AddDebugMesh(const std::string& name, const std::string& meshFile)
{
	debugMeshes.emplace(name, std::make_unique<MeshComponent>(meshFile, "test.png"));
	debugMeshes[name]->Create();
}
