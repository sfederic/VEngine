#pragma once

#include "../Widget.h"

//Shows input key icon to act as a tutorial for input.
class InputIconWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetIconImageFilename(std::string_view filename) { iconImageFilename = filename; }

private:
	std::string iconImageFilename;
	float alpha = 0.5f;
};
