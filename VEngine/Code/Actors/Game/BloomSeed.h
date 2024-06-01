#pragma once

#include "GridActor.h"

class MeshComponent;

//See that blooms into a flower platform when planted.
class BloomSeed : public GridActor
{
public:
	ACTOR_SYSTEM(BloomSeed);

	BloomSeed();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void Plant();

private:
	MeshComponent* flowerBloomMesh = nullptr;

	bool isPlanted = false;
};
