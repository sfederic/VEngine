#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class MeshComponent;

class TargetableMesh : public Actor
{
public:
	ACTOR_SYSTEM(TargetableMesh);

	TargetableMesh();
	void Create() override;
	Properties GetProps() override { return __super::GetProps(); }

private:
	MeshComponent* mesh = nullptr;
};
