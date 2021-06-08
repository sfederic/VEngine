#pragma once

#include <qdockwidget.h>
#include <unordered_map>

class World;
class ActorSystem;
class Actor;
class PropertiesWidget;
class QVBoxLayout;

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock(const char* title);

	virtual void enterEvent(QEvent* event);

	void Tick();
	void DisplayActorSystemProperties(Actor* actor);

	PropertiesWidget* propWidget;
	ActorSystem* selectedActorSystem;
	
	QWidget* actorPropsWidget;
};
