#pragma once

#include "../Widget.h"

class ReconWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetReconText(std::wstring_view text) { reconDisplayText = text; }

private:
	std::wstring reconDisplayText;
};
