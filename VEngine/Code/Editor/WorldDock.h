#pragma once
#include <qdockwidget.h>
#include <qabstractitemview.h>

class QTreeWidget;
class QTreeWidgetItem;
class QLineEdit;

//Holds a list of all the actors (and their parent child relationships) currently in world
struct WorldDock : public QDockWidget
{
	QTreeWidget* actorTreeWidget = nullptr;
	QLineEdit* actorSearchBar = nullptr;

private:
	QAbstractItemView::SelectionMode actorListSelectionMode;

public:
	WorldDock();
	void Tick();
	void PopulateWorldActorList();
	void SelectActorInList();

private:
	void ClickOnActorInList(QTreeWidgetItem* item, int column);
	void SearchActors();
	void ActorNameChanged(QTreeWidgetItem* item, int column);
	void ActorListContextMenu(const QPoint& pos);
};
