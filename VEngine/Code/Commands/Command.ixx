export module Commands.Command;

import Commands.ICommand;
import Core.Property;
import Core.World;
import Core.UID;

export template <typename T>
struct Command : public ICommand
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
		auto actor = World::Get().Get().GetActorByUIDAllowNull(prop.ownerUID);
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
