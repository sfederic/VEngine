#pragma once

#include <map>
#include <string>

struct Memory;

class MemorySystem
{
public:
	static MemorySystem& Get()
	{
		static MemorySystem instance;
		return instance;
	}

	void CreateMemory(std::string name, Memory* memory) 
	{
		memories.emplace(name, memory);
	}

	Memory* FindMemory(std::string name) 
	{
		auto memIt = memories.find(name);
		if (memIt != memories.end()) 
		{
			return memIt->second;
		}
		return nullptr;
	}

private:
	std::map<std::string, Memory*> memories;
};
