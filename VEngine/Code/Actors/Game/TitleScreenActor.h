#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class TitleScreenWidget;

//An empty actor to throw into the titlescreen map and display a titlescreen widget
class TitleScreenActor : public Actor
{
public:
	ACTOR_SYSTEM(TitleScreenActor);

	TitleScreenActor();
	virtual void Start() override;
	virtual Properties GetProps() override;

private:
	TitleScreenWidget* titleScreenWidget = nullptr;
};
