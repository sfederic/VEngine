#pragma once
#include "Widget.h"
#include <functional>

//Shows a Yes/No widget when player is interacting with MemoryTriggers
struct AcceptMemoryWidget : Widget
{
	std::string actorName;

	std::function<void(std::string)> acceptButtonCallback;
	//std::function<void(std::string)> declineButtonCallback;

	virtual void Draw(float deltaTime) override;
	void Reset();
};
