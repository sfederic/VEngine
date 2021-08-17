#pragma once
#include <qdockwidget.h>

struct Actor;
class QWidget;
class QGridLayout;

struct PropertiesDock : public QDockWidget
{
	QWidget* actorPropsWidget = nullptr;
	QGridLayout* actorPropsGridLayout = nullptr;

	PropertiesDock();
	void DisplayActorProperties(Actor* actor);
};
