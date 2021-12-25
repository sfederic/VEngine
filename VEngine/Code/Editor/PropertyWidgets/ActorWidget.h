#pragma once
#include "IPropertyWidget.h"
#include <qlineedit.h>

struct Actor;

//@Todo: actorwidget doesn't really work because it's trying to attribute an existing actor name to
//a pointer, but actor names are deserialised at the same time actor pointers are, so it does nothing.

//Is essentially a StringWidget that uses that finds the actor by its name.
struct ActorWidget : QLineEdit, IPropertyWidget
{
	Actor* actor = nullptr;

	ActorWidget(Property prop_);
	virtual void SetValue();
	virtual void ResetValue() override;

	void FindActor();
};
