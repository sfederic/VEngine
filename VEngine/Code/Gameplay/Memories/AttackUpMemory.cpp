#include "vpch.h"
#include "../Memory.h"

struct AttackUpMemory : public Memory
{
	AttackUpMemory() :
		Memory("Attack Up",
			"Increases attack by 1",
			"",
			"",
			"") {}
}instance;
