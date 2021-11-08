#pragma once
#include <qdockwidget.h>
#include <string>

struct QFileSystemModel;
struct QTreeView;
struct QListWidget;
struct QLineEdit;

struct AssetDock : public QDockWidget
{
    QFileSystemModel* fileSystemModel;
    QTreeView* assetTreeView;
    QListWidget* assetIcons;
    QLineEdit* assetFilterLineEdit;

    AssetDock();
    void AssetItemClicked();
    void AssetFolderClicked();
    void ShowContextMenu(const QPoint&);
    void FilterAssets();

    //Asset open functions
    void OpenAssetItemInDefaultProgram();
    void MeshFileClicked(const std::string meshFilename);
    void TextureFileClicked(const std::wstring textureFilename);
};
