#include "vpch.h"
#include "../Memory.h"
#include "Gameplay/GameUtils.h"

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

struct Symbol4Memory : public Memory
{
	Symbol4Memory() :
		Memory("Symbol 4",
			"Aden committed the	symbol to memory, after knowing Symbol 1 and 2.",
			"",
			"",
			"") {}
	virtual bool RunCondition() override
	{
		return GameUtils::CheckIfMemoryExists("Symbol 1") && GameUtils::CheckIfMemoryExists("Symbol 2");
	}
}instance4;

struct Symbol5Memory : public Memory
{
	Symbol5Memory() :
		Memory("Symbol 5",
			"Aden committed the	symbol to memory, after knowing Symbol 3 and 1.",
			"",
			"",
			"") {}
	virtual bool RunCondition() override
	{
		return GameUtils::CheckIfMemoryExists("Symbol 3") && GameUtils::CheckIfMemoryExists("Symbol 3");
	}
}instance5;
