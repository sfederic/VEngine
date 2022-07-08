#pragma once

//Base class for scripts
class Script
{
	virtual void Start() = 0;
	virtual void Tick(float deltaTime) = 0;
};