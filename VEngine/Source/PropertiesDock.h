#pragma once

#include <qdockwidget.h>

class World;
class ActorSystem;
class PropertiesWidget;

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock(const char* title);
	void Tick();

	void DisplayActorSystemProperties(ActorSystem* actorSystem);

	PropertiesWidget* propWidget;

	ActorSystem* selectedActorSystem;
};

