#pragma once

#include "GridActor.h"

class MemoryActor : public GridActor
{
public:
	ACTOR_SYSTEM(MemoryActor);

	void Create() override;
	Properties GetProps() override;

	void Interact() override {}
	virtual bool CheckMemory();

protected:
	void SetMemoryName(std::string_view memoryName);
	void AddOnInteract(bool add);

private:
	std::string memoryName;
};
