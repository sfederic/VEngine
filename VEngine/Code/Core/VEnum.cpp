#include "vpch.h"
#include "VEnum.h"
#include <cassert>

void VEnum::SetValue(const std::string& newValue)
{
	auto dataIt = data.find(newValue);
	assert(dataIt != data.end());
	value = newValue;
}

void VEnum::Add(const std::string& name)
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
	for (auto& dataPair : data)
	{
		names.push_back(dataPair.first);
	}
	return names;
}

bool VEnum::Contains(std::string value)
{
	return data.find(value) == data.end();
}

bool VEnum::Compare(std::string valueToCompare)
{
	return valueToCompare == value;
}

void VEnum::Reset()
{
	dataIndex = 0;
	data.clear();
	value.clear();
}
