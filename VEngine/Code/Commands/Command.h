#pragma once

#include "ICommand.h"
#include "Core/Property.h"
#include "Core/World.h"

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
		auto actor = World::GetActorByUIDAllowNull(prop.ownerUID);
		if (actor == nullptr)
		{
			return;
		}

		auto allProps = actor->GetAllProps();
		for (auto& props : allProps)
		{
			T* data = props.GetDataAllowNull<T>(prop.name);
			if (data)
			{
				if (prop.change)
				{
					prop.change(prop);
				}

				*data = value;
			}
		}
	}
};
