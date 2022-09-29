#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

class AOELine : public Actor
{
public:
	ACTOR_SYSTEM(AOELine);

	AOELine();
	virtual Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};
