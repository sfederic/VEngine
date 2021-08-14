#pragma once
#include <qdockwidget.h>

struct WorldDock : public QDockWidget
{
	struct QTreeWidget* actorTreeWidget;

	WorldDock();
};
