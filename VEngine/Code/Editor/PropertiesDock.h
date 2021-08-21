#pragma once
#include <qdockwidget.h>
#include <vector>

struct Actor;
struct IPropertyWidget;
struct Properties;
class QWidget;
class QGridLayout;

struct PropertiesDock : public QDockWidget
{
	QWidget* actorPropsWidget = nullptr;
	QGridLayout* actorPropsGridLayout = nullptr;

	std::vector<IPropertyWidget*> propertyWidgetsToUpdate;

	Actor* previousActor = nullptr;

	PropertiesDock();
	void DisplayActorProperties(Actor* actor);
	void IterateOverProperties(Properties& props, int& currentGridRow);
};
