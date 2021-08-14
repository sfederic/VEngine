#pragma once
#include <qdockwidget.h>

struct AssetDock : public QDockWidget
{
    struct QFileSystemModel* fileSystemModel;
    struct QTreeView* assetTreeView;
    struct QListWidget* assetIcons;

    AssetDock();
    void AssetItemClicked();
    void AssetFolderClicked();
};
