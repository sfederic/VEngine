#pragma once
#include <unordered_map>
#include <string>
#include <vector>

struct MeshComponent;

struct InstanceMeshSystem
{
	std::unordered_map<std::string, std::vector<MeshComponent*>> meshInstanceGroupingMap;

	void SortMeshInstances();
};

extern InstanceMeshSystem instanceMeshSystem;
