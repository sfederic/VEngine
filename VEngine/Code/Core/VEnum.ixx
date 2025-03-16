export module Core.VEnum;

import Core.Log;
import <string>;
import <vector>;
import <map>;
import <cassert>;

export class VEnum
{
public:
	std::string GetValue() { return value; }
	void SetValue(const std::string& newValue);
	void Add(const std::string& name);
	std::vector<std::string> GetAllNames();
	bool Contains(std::string _value);
	bool Compare(std::string valueToCompare);
	void Reset();

private:
	int dataIndex = 0;
	std::map<std::string, int> data;
	std::string value;
};

void VEnum::SetValue(const std::string& newValue)
{
	auto dataIt = data.find(newValue);
	if (dataIt == data.end())
	{
		Log("Could not find [%s] value for VEnum.\n", newValue.c_str());
		return;
	}
	value = newValue;
}

void VEnum::Add(const std::string& name)
{
	assert(data.find(name) == data.end());
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
		names.emplace_back(dataPair.first);
	}
	return names;
}

bool VEnum::Contains(std::string _value)
{
	return data.find(_value) == data.end();
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
