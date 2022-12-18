#include "vpch.h"
#include "Component.h"
#include "Log.h"

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
