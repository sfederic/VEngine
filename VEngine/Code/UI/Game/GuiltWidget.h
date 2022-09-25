#pragma once
#include "../Widget.h"

//Displays a 'dirt mask' relative to the player's guilt over combat.
//There are 3 levels of guilt for killing 'civilians'. Each increment changes the image and the third level 
//grants a game over.
struct GuiltWidget : Widget
{
private:
	uint32_t guiltLevel = 0;

public:
	static inline const int MAX_GUILT = 3;

	virtual void Draw(float deltaTime) override;
	void AddGuilt();
};

//Always shown alongside GuiltWidget for a few seconds to tell player what's going on.
struct GuiltPopup : Widget
{
	virtual void Draw(float deltaTime) override;
};
