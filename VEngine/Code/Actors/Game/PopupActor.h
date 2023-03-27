#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

class PopupWidget;

//Holds logic for displaying a popupwidget when needed on level load or other event.
class PopupActor : public Actor
{
public:
	ACTOR_SYSTEM(PopupActor);

	PopupActor();
	virtual void Start() override;
	virtual Properties GetProps() override;

private:
	PopupWidget* popupWidget = nullptr;

	std::wstring popupText;

	bool displayOnLevelLoad = true;
};
