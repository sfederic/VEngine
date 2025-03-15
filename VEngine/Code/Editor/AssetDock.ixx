export module Editor.AssetDocl;

#include <qdockwidget.h>

import <string>;

class QFileSystemModel;
class QTreeView;
class QListWidget;
class QPushButton;
class QLineEdit;
class Material;
class AssetIconListWidget;

export class AssetDock : public QDockWidget
{
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
	void SerialiseMaterialPropsToFile(Material& material);

	void ImportMesh();
	void ImportAnim();

	void SetDirectoriesToFilter();

	QFileSystemModel* fileSystemModel = nullptr;
	QTreeView* assetTreeView = nullptr;
	AssetIconListWidget* assetIconListWidget = nullptr;
	QLineEdit* assetFilterLineEdit = nullptr;
	QPushButton* importMeshButton = nullptr;
	QPushButton* importAnimButton = nullptr;
};
