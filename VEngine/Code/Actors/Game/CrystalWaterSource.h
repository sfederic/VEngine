#pragma once

#include "GridActor.h"

class WaterSource;

//Lore-wise, water in the aqueducts is 'generated' by these crystals. 
class CrystalWaterSource : public GridActor
{
public:
	ACTOR_SYSTEM(CrystalWaterSource);

	void Create() override;
	void Start() override;
	Properties GetProps() override;

	void Activate() override;
	void Deactivate() override;

private:
	std::string waterSourceName;
	WaterSource* waterSource = nullptr;
};
