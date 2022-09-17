#pragma once

#include "../Widget.h"

class NoteWidget : public Widget
{
public:
	virtual void Draw(float deltaTime) override;

	std::wstring noteText;
};
