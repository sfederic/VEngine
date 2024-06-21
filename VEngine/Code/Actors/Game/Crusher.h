#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;

//Todo: actor needs more work with lerping movement values and actually registering that its crushed actors
//via box/ray casts.

//Think like that Attack of the Clones scene in the factory. Moves back and forth and crushes things.
class Crusher : public Actor
{
public:
	ACTOR_SYSTEM(Crusher);

	Crusher();
	void Create() override;
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

private:
	XMVECTOR nextPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR originalPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	MeshComponent* mesh = nullptr;
	float moveTimer = 0.f;
	float retractTimer = 0.f;
	bool retracted = true;
};
