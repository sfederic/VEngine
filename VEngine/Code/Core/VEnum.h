#pragma once

#include <map>
#include <string>

class VEnum
{
public:
	std::string GetValue() { return value; }
	void SetValue(const std::string& newValue);
	void Add(const std::string& name);
	std::vector<std::string> GetAllNames();
	bool Contains(std::string value);
	bool Compare(std::string valueToCompare);

private:
	int dataIndex = 0;
	std::map<std::string, int> data;
	std::string value;
};
