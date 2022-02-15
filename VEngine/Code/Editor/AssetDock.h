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
    void ActorTemplateFileClicked(const std::string actorTemplateFilename);
    void TextureFileClicked(const std::wstring textureFilename);

private:
    //Create asset functions
    void CreateNewMapFile();
    void CreateNewDialogueFile();

    //Select the picked actor in the editor and create a template from it by writing its properties and 
    //component properties to file.
    void CreateNewActorTemplateFile();
};
