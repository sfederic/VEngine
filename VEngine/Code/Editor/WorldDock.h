#pragma once

#include <qdockwidget.h>
#include <qabstractitemview.h>

class ActorTreeWidget;
class QTreeWidgetItem;
class QLineEdit;
class QComboBox;
class Actor;

//Holds a list of all the actors (and their parent child relationships) currently in world
class WorldDock : public QDockWidget
{
public:
	WorldDock();
	void Tick();
	void PopulateWorldActorList();
	void SelectActorInList();
	void AddActorToList(Actor* actor);
	void RemoveActorFromList();

private:
	//Click on list to select actor(s)
	void ClickOnActorInList(QTreeWidgetItem* item, int column);

	//navigate with arrow keys to select (single) actor
	void ArrowSelectActorInList();

	void SearchActors();
	void ActorNameChanged(QTreeWidgetItem* item, int column);
	void ActorListContextMenu(const QPoint& pos);

	void ActorTypeFilterChanged(int index);

private:
	ActorTreeWidget* actorTreeWidget = nullptr;
	QLineEdit* actorSearchBar = nullptr;

	QComboBox* actorTypeComboBox = nullptr;

	QAbstractItemView::SelectionMode actorListSelectionMode =
		QAbstractItemView::SelectionMode::SingleSelection;
};
