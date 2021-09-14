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
    void ShowContextMenu(const QPoint&);

    //Asset open functions
    void OpenAssetItemInDefaultProgram();
    void MeshFileClicked(const std::string meshFilename);
    void TextureFileClicked(const std::wstring textureFilename);
    void MaterialFileClicked(const std::string materialFilename);

    //Asset file create functions
    void CreateMaterialFile();
};
