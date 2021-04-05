#pragma once

#include <qdockwidget.h>

class World;
class ActorSystem;
class Actor;
class PropertiesWidget;

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock(const char* title);

	virtual void enterEvent(QEvent* event);

	void Tick();
	void DisplayActorSystemProperties(Actor* actor);

	PropertiesWidget* propWidget;

	ActorSystem* selectedActorSystem;
};
