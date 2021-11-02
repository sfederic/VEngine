#pragma once
#include <qdockwidget.h>

class QTreeWidget;
class QTreeWidgetItem;
class QLineEdit;

//Holds a list of all the actors (and their parent child relationships) currently in world
struct WorldDock : public QDockWidget
{
	QTreeWidget* actorTreeWidget = nullptr;
	QLineEdit* actorSearchBar = nullptr;

	WorldDock();
	void PopulateWorldActorList();
	void SelectActorInList();

private:
	void ClickOnActorInList(QTreeWidgetItem* item, int column);
	void SearchActors();
	void ActorNameChanged(QTreeWidgetItem* item, int column);
};
