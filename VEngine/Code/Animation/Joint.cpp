#include "vpch.h"
#include "Joint.h"

void Joint::SetName(const char* name_)
{
	strcpy_s(name, sizeof(name), name_);
}
