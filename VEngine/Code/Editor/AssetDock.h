#pragma once

#include <qdockwidget.h>
#include <string>

struct QFileSystemModel;
struct QTreeView;
struct QListWidget;
struct QLineEdit;

struct AssetDock : public QDockWidget
{
private:
    QFileSystemModel* fileSystemModel;
    QTreeView* assetTreeView;
    QListWidget* assetIcons;
    QLineEdit* assetFilterLineEdit;

public:
    AssetDock();
    void AssetItemClicked();
    void AssetFolderClicked();
    void ShowContextMenu(const QPoint&);
    void FilterAssets();

private:
    //Asset open functions
    void OpenAssetItemInDefaultProgram();
    void MeshFileClicked(const std::string meshFilename);
    void ActorTemplateFileClicked(const std::string actorTemplateFilename);
    void TextureFileClicked(const std::wstring textureFilename);
    void MaterialFileClicked(const std::string materialFilename);

    //Create asset functions
    void CreateNewMapFile();
    void CreateNewDialogueFile();

    //Select the picked actor in the editor and create a template from it by writing its properties and 
    //component properties to file.
    void CreateNewActorTemplateFile();

    void CreateNewMaterialFile();
};
