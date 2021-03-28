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
	void Tick();

	void DisplayActorSystemProperties(Actor* actorSystem);

	PropertiesWidget* propWidget;

	ActorSystem* selectedActorSystem;
};

