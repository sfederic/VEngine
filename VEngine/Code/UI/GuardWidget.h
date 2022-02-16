#pragma once
#include "Widget.h"

//Shows guard information when enemies are attacking player
struct GuardWidget : Widget
{
private:
	enum class GuardState
	{
		None,
		Success,
		Fail
	} guardState;

public:
	virtual void Draw(float deltaTime) override;
	inline void SetGuardSuccess() { guardState = GuardState::Success; }
	inline void SetGuardFail() { guardState = GuardState::Fail; }
	inline void ResetGuard() { guardState = GuardState::None; }
};
