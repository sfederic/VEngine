#pragma once
#include <qdockwidget.h>

class QTreeWidget;
class QTreeWidgetItem;

//Displays information for all Actor and Component systems
struct SystemDock : public QDockWidget
{
	QTreeWidget* actorSystemTreeWidget;
	QTreeWidget* componentSystemTreeWidget;

	SystemDock();
	void PopulateSystemLists();
	void AddActorSystemsToWidget();
	void AddComponentSystemsToWidget();
	void ClickOnActorSystemItem(QTreeWidgetItem* item, int column);
	void ClickOnComponentSystemItem(QTreeWidgetItem* item, int column);
};
