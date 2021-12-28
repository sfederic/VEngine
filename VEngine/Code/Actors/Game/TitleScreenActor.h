#pragma once
#include "../Actor.h"
#include "../ActorSystem.h"

struct TitleScreenWidget;

//An empty actor to throw into the titlescreen map and display a titlescreen widget
struct TitleScreenActor : Actor
{
	ACTOR_SYSTEM(TitleScreenActor);

	TitleScreenWidget* titlescreenWidget = nullptr;

	TitleScreenActor();
	virtual void Start() override;
	virtual Properties GetProps() override;
};
