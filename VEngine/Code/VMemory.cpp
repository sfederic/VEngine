#include "VMemory.h"

uint64_t AllocSystem::GetAllocationSystemMemoryUsage()
{
	uint64_t total = 0;

	for (auto memIt : memoryMap)
	{
		MemoryItem& item = memIt.second;
		total += sizeof(MemoryItem);
		total += item.allocFunction.size();
		total += item.allocFile.size();
	}

	return total;
}

double AllocationPerType::GetTotalSizeInKB()
{
	return static_cast<double>(totalSize) / 1024.0;
}

double AllocationPerType::GetTotalSizeInMB()
{
	double kbSize = GetTotalSizeInKB();
	return kbSize / 1024.0;
}
