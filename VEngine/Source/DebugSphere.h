#pragma once

#include "Actor.h"

//Debug representation of bounding sphere for actors.
class DebugSphere : public ActorSystem
{
public:
	DebugSphere();
	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void SpawnActors(int numToSpawn) {}
	virtual Actor* SpawnActor(Transform transform) { return nullptr; }

	//These transforms are linked to DebugDraw::DrawSphere
	std::vector<Transform> debugSphereTransforms;
};

extern DebugSphere debugSphere;
