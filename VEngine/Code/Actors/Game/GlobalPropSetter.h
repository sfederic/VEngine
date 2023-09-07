#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

//This is an empty Actor that when its Activate() method is called, 
//it will set a defined global property and value.
//Right now it's only working with bools.
class GlobalPropSetter : public Actor
{
public:
	ACTOR_SYSTEM(GlobalPropSetter);

	Properties GetProps() override;
	void Activate() override;

private:
	std::string propertyName;
	bool propertyValue;
};
