#pragma once

#include <qdockwidget.h>

class WorldDock : public QDockWidget
{
public: 
	WorldDock(const char* title);

	class WorldWidget* worldWidget;
	class QListView* worldListView;
};

