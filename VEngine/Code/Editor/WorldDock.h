#pragma once
#include <qdockwidget.h>
#include <qabstractitemview.h>

class ActorTreeWidget;
class QTreeWidgetItem;
class QLineEdit;

//Holds a list of all the actors (and their parent child relationships) currently in world
struct WorldDock : public QDockWidget
{
	ActorTreeWidget* actorTreeWidget = nullptr;
	QLineEdit* actorSearchBar = nullptr;

private:
	QAbstractItemView::SelectionMode actorListSelectionMode;

public:
	WorldDock();
	void Tick();
	void PopulateWorldActorList();
	void SelectActorInList();

private:
	//Click on list to select actor(s)
	void ClickOnActorInList(QTreeWidgetItem* item, int column);

	//navigate with arrow keys to select (single) actor
	void ArrowSelectActorInList();

	void SearchActors();
	void ActorNameChanged(QTreeWidgetItem* item, int column);
	void ActorListContextMenu(const QPoint& pos);
};
