#include "vpch.h"
#include "Component.h"
#include "Core/Log.h"
#include "Core/World.h"

Properties Component::GetProps()
{
	Properties props;

	props.ownerUID = GetOwnerUID();

	//Because the serialise file format changed for components, had to put the Name and OwnerUID
	//separate from the other props so that components can find their owners on spawn (when defined in c++).
	//Leaving this here just for rough clarity to what it used to be.

	//props.Add("Name", &name).hide = true;
	//props.Add("OwnerUID", &ownerUID).hide = true;
	props.Add(" Enabled", &active);
	props.Add(" Visible", &visible);
	props.Add("UID", &uid).hide = true;

	return props;
}

std::string Component::GetTypeName()
{
	return componentSystem->GetName();
}

void Component::AddTag(const std::string& tag)
{
	if (tags.find(tag) != tags.end())
	{
		Log("Tag [%s] on Component [%s] already exists.", tag.c_str(), name.c_str());
	}

	tags.emplace(tag);
}

bool Component::HasTag(const std::string& tag)
{
	return tags.find(tag) != tags.end();
}

Actor* Component::GetOwner()
{
	Actor* owner = World::GetActorByUID(ownerUID);
	return owner;
}
