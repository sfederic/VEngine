#pragma once
#include <unordered_map>

//Because of how rough serialisation and editor UI widgets are with C++'s enums without reflection,
//this struct acts as a makeshift enum to help with that. It also acts as a base class, meaning
//you can inherit from it and define the enum's values as static strings in the struct.
struct VEnum
{
private:
	int dataIndex = 0;
	std::unordered_map<std::string, int> data;
	std::string value;

public:
	inline std::string GetValue() { return value; }

	void SetValue(const std::string newValue);
	void Add(const std::string name);
	auto GetAllNames();
	bool Compare(const std::string valueToCompare);
};
