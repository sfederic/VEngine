#pragma once

#include "GridActor.h"

class InputIconWidget;

//Grid actor that sets widget when player link selection is hovering over this actor.
class InputTutorialGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(InputTutorialGridActor);

	void Start() override;

	void OnPlayerLinkHover() override;
	void OnPlayerLinkHoverOff() override;

private:
	InputIconWidget* inputIconWidget = nullptr;
};
