#pragma once
#include <qdockwidget.h>

class QTreeWidget;

//Displays information for all Actor and Component systems
struct SystemDock : public QDockWidget
{
	QTreeWidget* actorSystemTreeWidget;
	QTreeWidget* componentSystemTreeWidget;

	SystemDock();
	void PopulateSystemLists();
	void AddActorSystemsToWidget();
	void AddComponentSystemsToWidget();
	void ClickOnActorSystem();
	void ClickOnComponentSystem();
};
