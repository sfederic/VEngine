#pragma once
#include <unordered_map>
#include <string>

struct System;

struct SystemCache
{
	std::unordered_map<std::string, System*>* nameToSystemMap = nullptr;

	void Add(System* system);
	System* Get(std::string systemName);
};

extern SystemCache systemCache;
