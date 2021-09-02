#pragma once
#include <qdockwidget.h>

class QTreeWidget;
class QTreeWidgetItem;

//Holds a list of all the actors (and their parent child relationships) currently in world
struct WorldDock : public QDockWidget
{
	QTreeWidget* actorTreeWidget;

	WorldDock();
	void PopulateWorldActorList();
	void ClickOnActorInList(QTreeWidgetItem* item, int column);
	void ActorNameChanged(QTreeWidgetItem* item, int column);
};
