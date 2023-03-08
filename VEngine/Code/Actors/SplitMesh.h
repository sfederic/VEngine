#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"
#include "Render/Vertex.h"

class MeshComponent;

//A part of a sliced mesh
class SplitMesh : public Actor
{
public:
	ACTOR_SYSTEM(SplitMesh);

	SplitMesh();
	void Tick(float deltaTime) override;
	void CreateSplitMesh(std::vector<Vertex>& meshVerts, MeshComponent* originalMesh);
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
	float destroyTimer = 0.f;
};
