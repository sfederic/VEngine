#pragma once

#include "Actor.h"
#include "ActorSystem.h"

class MeshComponent;

class MeshSplitActor : public Actor
{
public:
	ACTOR_SYSTEM(MeshSplitActor);

	MeshSplitActor();
	void Tick(float deltaTime) override;
	Properties GetProps() { return __super::GetProps(); }

private:
	MeshComponent* mesh = nullptr;

	std::vector<Vertex> mesh0Verts;
	std::vector<Vertex> mesh1Verts;
};
