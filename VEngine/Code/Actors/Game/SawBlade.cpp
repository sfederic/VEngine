#include "vpch.h"
#include "SawBlade.h"

void SawBlade::PowerUp()
{
	AddRotation(GetUpVectorV(), 2.5f);
}
