#pragma once
#include <qdockwidget.h>

class QTreeWidget;

struct WorldDock : public QDockWidget
{
	QTreeWidget* actorTreeWidget;

	WorldDock();
	void PopulateWorldActorList();
};
