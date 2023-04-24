#pragma once

#include "GridActor.h"

class MemoryActor : public GridActor
{
public:
	ACTOR_SYSTEM(MemoryActor);

	void Create() override;
	Properties GetProps() override;

	void Interact() override {}

protected:
	void SetMemoryName(std::wstring_view memoryName);

private:
	std::string memoryName;
};
