#pragma once
#include "IPropertyWidget.h"
#include <qlineedit.h>

struct Actor;

//Is essentially a StringWidget that uses that finds the actor by its name.
struct ActorWidget : QLineEdit, IPropertyWidget
{
	Actor* actor = nullptr;

	ActorWidget(Property prop_);
	virtual void SetValue();
	virtual void ResetValue() override;

	void FindActor();
};
