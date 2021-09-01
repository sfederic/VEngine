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
    void MeshFileClicked(std::string meshFilename);
    void ActorTemplateClicked(std::string actorTemplateFilename);
};
