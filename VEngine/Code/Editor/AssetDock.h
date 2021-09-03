#pragma once
#include <qdockwidget.h>
#include <string>

struct AssetDock : public QDockWidget
{
    struct QFileSystemModel* fileSystemModel;
    struct QTreeView* assetTreeView;
    struct QListWidget* assetIcons;

    AssetDock();
    void AssetItemClicked();
    void AssetFolderClicked();
    void OpenAssetItemInDefaultProgram();
    void MeshFileClicked(std::string meshFilename);
};
