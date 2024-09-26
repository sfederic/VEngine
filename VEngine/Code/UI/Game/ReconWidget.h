#pragma once

#include "../Widget.h"

class ReconWidget : public Widget
{
public:
	ReconWidget() : Widget("ReconWidget") {}
	void Draw(float deltaTime) override;
	void SetReconText(std::wstring_view text) { reconDisplayText = text; }

private:
	std::wstring reconDisplayText;
};
