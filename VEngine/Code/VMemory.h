#pragma once
#include <string>
#include <optional>
#include <typeindex>
#include <source_location>
#include <map>

//Good reference on memory and allocators from Bluepoint's Shadow of the Colossus remake
//REF:https://gdcvault.com/play/1026277/Beyond-the-Remake-of-Shadow

struct MemoryItem
{
	std::string allocFunction;
	std::string allocFile;
	std::optional<std::type_index> type;
	void* ptr;
	size_t size;
	uint32_t allocLine;
};

struct AllocationPerType
{
	uint64_t totalSize = 0;
	uint64_t numPerType = 0;

	double GetTotalSizeInKB();
	double GetTotalSizeInMB();
};

struct AllocSystem
{
	std::map<void*, MemoryItem> memoryMap;

	template <typename T>
	T* Alloc(std::source_location location = std::source_location::current())
	{
		T* ptr = new T;

#ifdef _DEBUG
		auto size = sizeof(T);

		assert(_msize(ptr) == size);

		MemoryItem item = {};
		item.ptr = ptr;
		item.size = size;
		item.type = typeid(T);
		item.allocFunction = location.function_name();
		item.allocLine = location.line();
		item.allocFile = location.file_name();
		memoryMap.emplace(ptr, item);
#endif

		return ptr;
	}

	template <typename T>
	void Dealloc(T* ptr)
	{
#ifdef _DEBUG
		assert(_msize(ptr) == sizeof(T));
		memoryMap.erase(ptr);
#endif
		delete ptr;
		ptr = nullptr;
	}

	template <typename T>
	AllocationPerType GetTotalAllocatedMemoryByType()
	{
		AllocationPerType total = {};

		for (auto memIt : memoryMap)
		{
			MemoryItem& item = memIt.second;

			if (item.type == typeid(T))
			{
				total.numPerType++;
				total.totalSize += item.size;
			}
		}

		return total;
	}

	uint64_t GetAllocationSystemMemoryUsage();
};

AllocSystem allocSystem;

template <typename T>
T* Alloc()
{
	return allocSystem.Alloc<T>();
}
