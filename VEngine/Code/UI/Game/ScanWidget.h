#pragma once

#include "../Widget.h"

class ScanWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	void ResetValues();

	void SetScanInfoText(std::wstring scanInfoText_) { scanInfoText = scanInfoText_; }
	std::wstring GetScanInfoText() { return scanInfoText; }

private:
	std::wstring scanInfoText;
};
