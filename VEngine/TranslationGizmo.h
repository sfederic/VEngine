#pragma once

#include "Actor.h"

enum class EPickedAxis
{
	X,
	Y,
	Z
};

class TranslationGizmo : public Actor
{
public:
	virtual void Tick(float deltaTime) override
	{

	}

	EPickedAxis pickedAxis;
};

class TranslationGizmoSystem : public ActorSystem
{
public:
	virtual void Tick(float deltaTime) override
	{

	}
};