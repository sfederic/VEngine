#pragma once

#include <qdockwidget.h>
#include "PropertiesWidget.h"

class World;
class ActorSystem;

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock(const char* title);
	void Tick();

	void DisplayActorSystemProperties(ActorSystem* actorSystem);

	PropertiesWidget* propWidget;
};

