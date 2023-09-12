#include "vpch.h"
#include "Crushable.h"

void Crushable::Crushed()
{
	Destroy();
}

void Crushable::Attacked()
{
	__super::Attacked();

	Destroy();
}
