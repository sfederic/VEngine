#include "vpch.h"
#include "System.h"
#include "SystemCache.h"

System::System(std::string name_) : name(name_)
{
	systemCache.Add(this);
}
