#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class PhotoComponent;
struct MeshComponent;

class PhotoActor : public Actor
{
public:
	ACTOR_SYSTEM(PhotoActor);

	PhotoActor();
	virtual Properties GetProps() override;

private:
	PhotoComponent* photoComponent = nullptr;

	MeshComponent* meshComponent = nullptr;
};
