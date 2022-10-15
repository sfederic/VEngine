#pragma once

#include "../Memory.h"

struct AttackUpMemory : public Memory
{
	static AttackUpMemory instance;

	AttackUpMemory() : 
		Memory("Attack Up",
		"Increases attack by 1",
		"",
		"",
		1, 0) {}
};
