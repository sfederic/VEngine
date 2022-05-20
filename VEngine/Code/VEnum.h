#pragma once
#include <unordered_map>

//Serialisation and editor UI widgets are with C++'s enums without reflection is rough,
//this struct acts as a makeshift enum. It also acts as a base class, meaning
//you can inherit from it and define the enum's values as static strings in the struct, or just define
//static string values in a seperate struct and pass them into VEnum given that inheritance
//isn't supported in the engine's serialisation.
struct VEnum
{
private:
	int dataIndex = 0;
	std::unordered_map<std::string, int> data;
	std::string value;

public:
	std::string GetValue() { return value; }

	void SetValue(const std::string newValue);
	void Add(const std::string& name);
	std::vector<std::string> GetAllNames();
	bool Compare(const std::string valueToCompare);
};
