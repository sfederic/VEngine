#pragma once
#include <corecrt_malloc.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

//TODO: don't think I'll keep this. std::vector is just as fast, unless of course it beats it in compile times
//What's good is this gives all the byte size infomation. Maybe can just use  class that inherits from std::vector instead?
template <class T>
class Array
{
public:
	Array() {};
	Array(uint64_t initSize)
	{
		size = initSize;
		assert(elementSize < UINT_MAX);
		sizeInBytes = initSize * elementSize;

		const int smallestAllocSize = 32;

		if (sizeInBytes < smallestAllocSize)
		{
			maxSizeInBytes = smallestAllocSize; //32 being smallest alloc on 64bit systems? MSVC included?
			data = (T*)calloc(size, maxSizeInBytes);
		}
		else
		{
			maxSizeInBytes = sizeInBytes * 2;
			data = (T*)calloc(size, maxSizeInBytes);
		}
	}

	void add(T element)
	{
		size++;
		sizeInBytes = size * elementSize;

		//assert(data); //Assert doing funny things with realloc?

		if (sizeInBytes < maxSizeInBytes)
		{
			data[size - 1] = element;
		}
		else
		{
			maxSizeInBytes = sizeInBytes * 2;
			data = (T*)realloc(data, maxSizeInBytes);
			data[size - 1] = element;
		}
	}

	T* data;
	uint64_t sizeInBytes;
	uint64_t size;
	uint64_t maxSizeInBytes;
	unsigned int elementSize = sizeof(T);
};
