#pragma once

#include "../Widget.h"

//Shows currently taken photo from Player for in-game mechanics.
class PhotoWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	std::string photoFilename;
};
