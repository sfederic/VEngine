#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

class SkillNode : public Actor
{
public:
	ACTOR_SYSTEM(SkillNode);

	SkillNode();
	virtual Properties GetProps() override;

private:
	MeshComponent* mesh = nullptr;
};
