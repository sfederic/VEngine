#pragma once

#include "../Widget.h"

//Shows guard information when enemies are attacking player
class GuardWidget : public Widget
{
public:
	void Draw(float deltaTime) override;
	void SetGuardSuccess() { guardState = GuardState::Success; }
	void SetGuardFail() { guardState = GuardState::Fail; }
	void ResetGuard() { guardState = GuardState::None; }

private:
	enum class GuardState
	{
		None,
		Success,
		Fail
	};

	GuardState guardState = GuardState::None;
};
