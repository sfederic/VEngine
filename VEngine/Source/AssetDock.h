#pragma once

#include <qdockwidget.h>

class QFileSystemModel;
class QTreeView;
class QModelIndex;
class QListWidget;

class AssetDock : public QDockWidget
{
public:
	AssetDock(const char* title);
	void Tick();
	void AssetItemClicked();
	void AssetFolderClicked();

	QListWidget* assetIcons;
	QFileSystemModel* fileSystemModel;
	QTreeView* assetTreeView;
	QModelIndex* modelIndex;
};
