#pragma once

#include <map>
#include <string>

class VEnum
{
public:
	std::string GetValue() { return value; }
	void SetValue(const std::string newValue);
	void Add(const std::string name);
	std::vector<std::string> GetAllNames();
	bool Contains(std::string value);
	bool Compare(std::string valueToCompare);

	//Use this instead of the default copy constructor.
	//Originally was causing debug iterator issues when deleting materials.
	void Copy(const VEnum& VEnumToCopy)
	{
		dataIndex = VEnumToCopy.dataIndex;
		data = VEnumToCopy.data;
		value = VEnumToCopy.value;
	}

private:
	int dataIndex = 0;
	std::map<std::string, int> data;
	std::string value;
};
