#include "vpch.h"
#include "VEnum.h"
#include <cassert>

void VEnum::SetValue(const std::string newValue)
{
	auto dataIt = data.find(newValue);
	assert(dataIt != data.end());
	value = newValue;
}

void VEnum::Add(const std::string name)
{
	data.emplace(name, dataIndex);
	if (dataIndex == 0) //First Add(), set default value
	{
		SetValue(name);
	}
	dataIndex++;
}

std::vector<std::string> VEnum::GetAllNames()
{
	std::vector<std::string> names;
	for (auto& [name, index] : data)
	{
		names.emplace_back(name);
	}
	return names;
}

bool VEnum::Compare(const std::string valueToCompare)
{
	assert(data.find(valueToCompare) != data.end());
	return data.find(valueToCompare) == data.find(valueToCompare);
}
