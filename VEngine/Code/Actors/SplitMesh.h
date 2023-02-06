#pragma once

#include "Actors/Actor.h"
#include "Actors/ActorSystem.h"

class MeshComponent;
struct Transform;

//A part of a sliced mesh
class SplitMesh : public Actor
{
public:
	ACTOR_SYSTEM(SplitMesh);

	SplitMesh();
	void Tick(float deltaTime) override;
	void CreateSplitMesh(std::vector<Vertex>& meshVerts, const Transform& originalMeshTransform);
	Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
	float destroyTimer = 0.f;
};
