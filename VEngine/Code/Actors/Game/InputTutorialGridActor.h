#pragma once

#include "GridActor.h"

class InputIconWidget;

//Grid actor that sets widget when player link selection is hovering over this actor.
class InputTutorialGridActor : public GridActor
{
public:
	ACTOR_SYSTEM(InputTutorialGridActor);

	InputTutorialGridActor();
	void Start() override;
	void Tick(float deltaTime) override;
	Properties GetProps() override;

	void OnPlayerLinkHover() override;
	void OnPlayerLinkHoverOff() override;
	void OnLinkActivate() override;
	void OnLinkDeactivate() override;

private:
	std::string iconImage;
	InputIconWidget* inputIconWidget = nullptr;
};
