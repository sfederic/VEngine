#include "vpch.h"
#include "Crushable.h"

void Crushable::Crushed()
{
	Destroy();
}

void Crushable::Attacked()
{
	Destroy();
}
