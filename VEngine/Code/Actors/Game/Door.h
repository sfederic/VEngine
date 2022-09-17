#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct MeshComponent;

class Door : public Actor
{
public:	
	ACTOR_SYSTEM(Door);

	Door();
	virtual Properties GetProps() override;

	void Open();

private:
	MeshComponent* mesh = nullptr;

	bool isOpen = false;
};
