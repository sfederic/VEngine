#include "ActorWidget.h"
#include "Actors/Actor.h"
#include "World.h"

ActorWidget::ActorWidget(Property prop_)
{
	prop = prop_;
	actor = static_cast<Actor*>(prop.data);
	connect(this, &QLineEdit::editingFinished, this, &ActorWidget::SetValue);
	FindActor();
	ResetValue();
}

void ActorWidget::SetValue()
{
	FindActor();
}

void ActorWidget::ResetValue()
{
	if (actor)
	{
		setText(QString::fromStdString(actor->name));
	}
}

void ActorWidget::FindActor()
{
	std::string actorName = text().toStdString();
	Actor* foundActor = world.GetActorByNameAllowNull(actorName);
	if (foundActor)
	{
		actor = foundActor;
	}
}
