#pragma once

#include "ICommand.h"
#include "Core/Property.h"
#include "Core/UID.h"
#include "Core/World.h"
#include "Actors/Actor.h"

template <typename T>
struct Command : ICommand
{
	//Property prop;
	T value;
	Property prop;

	Command(Property& prop_)
	{
		prop = prop_;
		value = *prop.GetData<T>();
	}

	void Execute() override
	{
		auto actor = World::GetActorByUID(prop.ownerUID);
		auto props = actor->GetProps();
		auto data = props.GetData<T>(prop.name);
		*data = value;
	}
};
