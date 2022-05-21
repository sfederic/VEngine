#include "vpch.h"
#include "VEnum.h"
#include <cassert>

void VEnum::SetValue(const std::string newValue)
{
	auto dataIt = data.find(newValue);
	assert(dataIt != data.end());
	value = newValue;
}

void VEnum::Add(const std::string& name)
{
	data[name] = dataIndex;
	if (dataIndex == 0) //First Add(), set default value
	{
		SetValue(name);
	}
	dataIndex++;
}

std::vector<std::string> VEnum::GetAllNames()
{
	std::vector<std::string> names;
	for (auto& name : data)
	{
		names.push_back(name.first);
	}
	return names;
}

bool VEnum::Compare(const std::string valueToCompare)
{
	assert(data.find(valueToCompare) != data.end());
	return data[value] == data[valueToCompare];
}
