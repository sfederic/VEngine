#pragma once

#include <qdockwidget.h>
#include <string>

struct QFileSystemModel;
struct QTreeView;
struct QListWidget;
class QPushButton;
struct QLineEdit;
class Material;

struct AssetDock : public QDockWidget
{
private:
    QFileSystemModel* fileSystemModel;
    QTreeView* assetTreeView;
    QListWidget* assetIcons;
    QLineEdit* assetFilterLineEdit;
    QPushButton* importButton = nullptr;

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
    void CopyMaterialToMaterialFileFromSelectedActor();
    void SerialiseMaterialPropsToFile(Material* material);

    void ImportAsset();
};
