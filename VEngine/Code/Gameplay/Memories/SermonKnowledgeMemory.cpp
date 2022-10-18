#include "vpch.h"
#include "../Memory.h"

struct SermonKnowledgeMemory : public Memory
{
	SermonKnowledgeMemory() : Memory(
		"Sermen Knowledge",
		"Aden now knows how the funeral will be conducted for the Bestowed.",
		"",
		"",
		""
	) {}
}instance;
