#pragma once

#include "Actor.h"

//Debug representation of bounding box for actors.
class DebugBox : public ActorSystem
{
public:
	DebugBox();
	virtual void Tick(float deltaTime);
	virtual void Start();
	virtual void SpawnActors(int numToSpawn) { }
	virtual Actor* SpawnActor(Transform transform) { return nullptr; }

	//These transforms are linked to DebugDraw::DrawBox
	std::vector<Transform> debugBoxTransforms;
};

extern DebugBox debugBox;
