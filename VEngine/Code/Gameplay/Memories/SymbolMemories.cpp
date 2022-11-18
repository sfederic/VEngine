#include "vpch.h"
#include "../Memory.h"

struct Symbol1Memory : public Memory
{
	Symbol1Memory() :
		Memory("Symbol 1",
			"Aden committed the	symbol to memory.",
			"",
			"",
			"") {}
}instance1;

struct Symbol2Memory : public Memory
{
	Symbol2Memory() :
		Memory("Symbol 2",
			"Aden committed the	symbol to memory.",
			"",
			"",
			"") {}
}instance2;

struct Symbol3Memory : public Memory
{
	Symbol3Memory() :
		Memory("Symbol 3",
			"Aden committed the	symbol to memory.",
			"",
			"",
			"") {}
}instance3;
