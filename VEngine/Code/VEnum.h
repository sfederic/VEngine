#pragma once

#include <map>

struct VEnum
{
private:
	int dataIndex = 0;
	std::map<std::string, int> data;
	std::string value;

public:
	std::string GetValue() { return value; }

	void SetValue(const std::string newValue);
	void Add(const std::string& name);
	std::vector<std::string> GetAllNames();
	bool Compare(const std::string valueToCompare);
};
