#include "WorldDock.h"
#include <qtreewidget.h>
#include <QTreeWidgetItem>

WorldDock::WorldDock() : QDockWidget("World")
{
	actorTreeWidget = new QTreeWidget();
	actorTreeWidget->setColumnCount(1);
	actorTreeWidget->setHeaderLabels(QStringList("Active Actor Systems"));

	auto item = new QTreeWidgetItem(actorTreeWidget);
	item->setText(0, "big dick actor");
	item->insertChild(0, new QTreeWidgetItem());

	setWidget(actorTreeWidget);
}