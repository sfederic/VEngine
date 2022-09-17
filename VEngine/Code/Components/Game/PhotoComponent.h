#pragma once

#include "../Component.h"
#include "../ComponentSystem.h"

//Component to hold photo data for in-game Actors in world.
class PhotoComponent : public Component
{
public:
	COMPONENT_SYSTEM(PhotoComponent);

	PhotoComponent() {}
	virtual Properties GetProps() override;

	bool IsTagPartOfCurrentSalvage();
	std::string GetPhotoTag() { return photoTag; }

private:
	//The name of the photographable object in-game mechanics are tied to.
	std::string photoTag;
};
