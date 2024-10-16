#pragma once

#include "GridActor.h"

//An Actor the player can recon while interacting in recon mode.
class ReconActor : public GridActor
{
public:
	ACTOR_SYSTEM(ReconActor);

	void Create() override;
	Properties GetProps() override;

	auto GetReconText() { return reconText; }

private:
	void SetReconText();

	std::string reconTextKey;
	std::wstring reconText;
};
