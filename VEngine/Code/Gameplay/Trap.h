#pragma once

#include <string>

struct GridNode;

struct Trap
{
	Trap(std::wstring name_, std::wstring desc_, std::string imageFilename_) :
		name(name_), desc(desc_), imageFilename(imageFilename_) {}

	virtual void Set();
	virtual void Activate() = 0;

	GridNode* connectedNode = nullptr;

	std::wstring name;
	std::wstring desc;
	std::string imageFilename;
};
