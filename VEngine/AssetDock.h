#pragma once

#include <qdockwidget.h>
#include <QModelIndex>

class AssetDock : public QDockWidget
{
public:
	AssetDock(const char* title);
	void Tick();
	void AssetItemClicked(class QListWidgetItem* listWidgetItem);

	QModelIndex* modelIndex;
};
