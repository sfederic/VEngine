#include "WorldDock.h"
#include "WorldWidget.h"
#include <qlistview.h>

WorldDock::WorldDock(const char* title) : QDockWidget(title)
{
    WorldWidget* worldWidget = new WorldWidget();

    QListView* worldListView = new QListView();

    setWidget(worldWidget);
}
