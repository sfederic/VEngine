#include "vpch.h"
#include "AssetDock.h"
#include <qdesktopservices.h>
#include <qmenu.h>
#include <qfiledialog.h>
#include <filesystem>
#include <qpushbutton.h>
#include <qdockwidget.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>
#include <qlistwidget.h>
#include <qboxlayout.h>
#include <qlineedit.h>
#include "Editor/AssetIconListWidget.h"
#include "Core/FileSystem.h"
#include "Asset/AssetSystem.h"
#include "Actors/MeshActor.h"
#include "Core/WorldEditor.h"
#include "Render/TextureSystem.h"
#include "Render/Material.h"
#include "Render/Renderer.h"
#include "Render/ShaderItem.h"
#include "Render/MaterialSystem.h"
#include "Gameplay/GameInstance.h"
#include "Core/Log.h"
#include "Asset/AssetBaseFolders.h"
#include "Asset/AssetFileExtensions.h"
#include "Components/MeshComponent.h"

std::unordered_map<std::string, std::function<void(QIcon&, std::string&)>> fileExtensionToFunctionMap;

namespace Icons
{
	QPixmap* play;
	QPixmap* font;
	QPixmap* icon;
	QPixmap* lib;
	QPixmap* mesh;
	QPixmap* fbx;
	QPixmap* world;
	QPixmap* code;
	QPixmap* material;
	QPixmap* dialogue;
	QPixmap* audio;
}

AssetDock::AssetDock() : QDockWidget("Assets")
{
	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QWidget::customContextMenuRequested, this, &AssetDock::ShowContextMenu);

	//Set icons
	Icons::play = new QPixmap("Icons/play_icon.png");
	Icons::font = new QPixmap("Icons/font_icon.png");
	Icons::icon = new QPixmap("Icons/icon_icon.png");
	Icons::lib = new QPixmap("Icons/lib_icon.png");
	Icons::mesh = new QPixmap("Icons/mesh_icon.png");
	Icons::fbx = new QPixmap("Icons/FBX_icon.png");
	Icons::world = new QPixmap("Icons/world_icon.png");
	Icons::code = new QPixmap("Icons/code_icon.png");
	Icons::material = new QPixmap("Icons/material_icon.png");
	Icons::dialogue = new QPixmap("Icons/dialogue_icon.png");
	Icons::audio = new QPixmap("Icons/audio_icon.png");

	fileSystemModel = new QFileSystemModel();
	fileSystemModel->setRootPath(QDir::currentPath());

	SetDirectoriesToFilter();

	assetTreeView = new QTreeView();
	assetTreeView->setModel(fileSystemModel);
	assetTreeView->setRootIndex(fileSystemModel->index(QDir::currentPath()));
	assetTreeView->setMaximumWidth(250);
	assetTreeView->setSortingEnabled(true);
	connect(assetTreeView, &QTreeView::clicked, this, &AssetDock::AssetFolderClicked);

	//Hide all columns except for 'Name' (Other columns mirror Win32 usuals. Date, Type, etc.)
	//Start on i = 1 as 'Name' is first column.

	for (int i = 1; i < fileSystemModel->columnCount(); i++)
	{
		assetTreeView->hideColumn(i);
	}

	assetIconListWidget = new AssetIconListWidget();
	connect(assetIconListWidget, &QListWidget::clicked, this, &AssetDock::AssetItemClicked);
	connect(assetIconListWidget, &QListWidget::doubleClicked, this, &AssetDock::OpenAssetItemInDefaultProgram);

	QHBoxLayout* assetHBox = new QHBoxLayout();
	assetHBox->addWidget(assetTreeView);
	assetHBox->addWidget(assetIconListWidget);

	//Setup search bar
	assetFilterLineEdit = new QLineEdit();
	assetFilterLineEdit->setPlaceholderText("Search Assets...");
	connect(assetFilterLineEdit, &QLineEdit::textChanged, this, &AssetDock::FilterAssets);

	//Setup Import buttons
	importMeshButton = new QPushButton("Import Mesh", this);
	connect(importMeshButton, &QPushButton::clicked, this, &AssetDock::ImportMesh);
	importAnimButton = new QPushButton("Import Anim", this);
	connect(importAnimButton, &QPushButton::clicked, this, &AssetDock::ImportAnim);

	auto assetToolbarHBox = new QHBoxLayout();
	assetToolbarHBox->addWidget(assetFilterLineEdit);
	assetToolbarHBox->addWidget(importMeshButton);
	assetToolbarHBox->addWidget(importAnimButton);

	auto vLayout = new QVBoxLayout();
	vLayout->addLayout(assetToolbarHBox);
	vLayout->addLayout(assetHBox);

	QWidget* assetWidget = new QWidget();
	assetWidget->setLayout(vLayout);

	setWidget(assetWidget);

	QFileSystemModel* fileModel = new QFileSystemModel();
	fileModel->setRootPath(QDir::currentPath());

	//Create file extension map
	fileExtensionToFunctionMap.emplace(".jpg", [&](QIcon& icon, std::string& filePath) {
		icon = QPixmap(VString::GetSubStringWithFoundOffset(filePath, AssetBaseFolders::texture).c_str());
		});
	fileExtensionToFunctionMap.emplace(".png", [&](QIcon& icon, std::string& filePath) {
		icon = QPixmap(VString::GetSubStringWithFoundOffset(filePath, AssetBaseFolders::texture).c_str());
		});
	fileExtensionToFunctionMap.emplace(".vmesh", [&](QIcon& icon, std::string& filePath) { icon = *Icons::mesh; });
	fileExtensionToFunctionMap.emplace(".fbx", [&](QIcon& icon, std::string& filePath) { icon = *Icons::fbx; });
	fileExtensionToFunctionMap.emplace(".ttf", [&](QIcon& icon, std::string& filePath) { icon = *Icons::font; });
	fileExtensionToFunctionMap.emplace(".lib", [&](QIcon& icon, std::string& filePath) { icon = *Icons::lib; });
	fileExtensionToFunctionMap.emplace(".dll", [&](QIcon& icon, std::string& filePath) { icon = *Icons::lib; });
	fileExtensionToFunctionMap.emplace(".vmap", [&](QIcon& icon, std::string& filePath) { icon = *Icons::world; });
	fileExtensionToFunctionMap.emplace(".vmat", [&](QIcon& icon, std::string& filePath) { icon = *Icons::material; });
	fileExtensionToFunctionMap.emplace(".dialog", [&](QIcon& icon, std::string& filePath) { icon = *Icons::dialogue; });
	fileExtensionToFunctionMap.emplace(".sav", [&](QIcon& icon, std::string& filePath) { icon = *Icons::world; });
	fileExtensionToFunctionMap.emplace(".wav", [&](QIcon& icon, std::string& filePath) { icon = *Icons::audio; });
	fileExtensionToFunctionMap.emplace(".h", [&](QIcon& icon, std::string& filePath) { icon = *Icons::code; });
	fileExtensionToFunctionMap.emplace(".cpp", [&](QIcon& icon, std::string& filePath) { icon = *Icons::code; });
}

void AssetDock::AssetItemClicked()
{
	QModelIndex index = assetTreeView->currentIndex();
	QString path = fileSystemModel->filePath(index);

	QString assetName = assetIconListWidget->currentItem()->text();
	QString fullPath = path + "/" + assetName;

	auto fileExtension = std::filesystem::path(fullPath.toStdString()).extension();
	auto extension = fileExtension.c_str();

	std::string assetPath = fullPath.toStdString();

	if (std::wcscmp(extension, L".vmap") == 0) //Map files 
	{
		//Reassign instance map data so entrance triggers don't mess up.
		GameInstance::startingMap = assetPath;
		GameInstance::previousMapMovedFrom = GameInstance::startingMap;

		assetPath = VString::GetSubStringAtFoundOffset(assetPath, AssetBaseFolders::worldMap);
		FileSystem::LoadWorld(assetPath);
	}
	else if (std::wcscmp(extension, L".vmesh") == 0) //Vmesh files
	{
		assetPath = VString::GetSubStringAtFoundOffset(assetPath, AssetBaseFolders::mesh);
		MeshFileClicked(assetPath);
	}
	else if (std::wcscmp(extension, L".actor") == 0) //Actor Template files
	{
		assetPath = VString::GetSubStringAtFoundOffset(assetPath, AssetBaseFolders::actorTemplate);
		ActorTemplateFileClicked(assetPath);
	}
	else if (std::wcscmp(extension, L".vmat") == 0) //Material files
	{
		assetPath = VString::GetSubStringAtFoundOffset(assetPath, AssetBaseFolders::material);
		MaterialFileClicked(assetPath);
	}
	else if (std::wcscmp(extension, L".jpg") == 0 || //Image files
		std::wcscmp(extension, L".png") == 0)
	{
		assetPath = VString::GetSubStringAtFoundOffset(assetPath, AssetBaseFolders::texture);
		TextureFileClicked(VString::stows(assetPath));
	}
}

void AssetDock::OpenAssetItemInDefaultProgram()
{
	QModelIndex index = assetTreeView->currentIndex();
	QString path = fileSystemModel->filePath(index);

	QString assetName = assetIconListWidget->currentItem()->text();
	QString fullPath = path + "/" + assetName;

	//Opens up default system program from filename.
	QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
}

void AssetDock::AssetFolderClicked()
{
	QModelIndex index = assetTreeView->currentIndex();
	QString folderPath = fileSystemModel->filePath(index);

	QDir directory(folderPath);
	QFileInfoList fileInfoList = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

	assetIconListWidget->clear();

	for (QFileInfo& fileInfo : fileInfoList)
	{
		QIcon icon;
		std::string filePath = fileInfo.absoluteFilePath().toStdString();
		auto fileExtension = std::filesystem::path(filePath).extension();

		auto extToFuncIt = fileExtensionToFunctionMap.find(fileExtension.string());
		if (fileExtension == ".vmesh")
		{
			std::string meshIconPath = "Icons/MeshIcons/" + fileInfo.fileName().toStdString() + ".jpg";
			if (std::filesystem::exists(meshIconPath))
			{
				icon = QPixmap(meshIconPath.c_str()).scaled(QSize(320, 240));
			}
			else
			{
				icon = *Icons::mesh;
			}
		}
		else if (fileExtension == ".vmap")
		{
			std::string mapIconPath = "Icons/MapIcons/" + fileInfo.fileName().toStdString() + ".jpg";
			if (std::filesystem::exists(mapIconPath))
			{
				icon = QPixmap(mapIconPath.c_str()).scaled(QSize(320, 240));
			}
			else
			{
				icon = *Icons::world;
			}
		}
		else if (extToFuncIt != fileExtensionToFunctionMap.end())
		{
			extToFuncIt->second(icon, filePath);
		}
		else
		{
			//Set default icon
			icon = *Icons::play;
		}

		QListWidgetItem* item = new QListWidgetItem(icon, fileInfo.fileName());
		item->setToolTip(fileInfo.fileName());
		item->setSizeHint(QSize(150, 100));
		assetIconListWidget->addItem(item);
	}
}

void AssetDock::ShowContextMenu(const QPoint& point)
{
	QMenu contextMenu("Context menu", this);

	QAction newMapAction("New Map", this);
	connect(&newMapAction, &QAction::triggered, this, &AssetDock::CreateNewMapFile);
	contextMenu.addAction(&newMapAction);

	QAction newDialogueAction("New Dialogue", this);
	connect(&newDialogueAction, &QAction::triggered, this, &AssetDock::CreateNewDialogueFile);
	contextMenu.addAction(&newDialogueAction);

	QAction newActorTemplateAction("New Actor Template", this);
	connect(&newActorTemplateAction, &QAction::triggered, this, &AssetDock::CreateNewActorTemplateFile);
	contextMenu.addAction(&newActorTemplateAction);

	QAction newMaterialAction("New Material", this);
	connect(&newMaterialAction, &QAction::triggered, this, &AssetDock::CreateNewMaterialFile);
	contextMenu.addAction(&newMaterialAction);

	QAction newMaterialPickedActorAction("Create Material From Picked Actor", this);
	connect(&newMaterialPickedActorAction, &QAction::triggered, this, &AssetDock::CopyMaterialToMaterialFileFromSelectedActor);
	contextMenu.addAction(&newMaterialPickedActorAction);

	contextMenu.exec(mapToGlobal(point));
}

void AssetDock::FilterAssets()
{
	QString filterText = assetFilterLineEdit->text().toLower();

	for (int i = 0; i < assetIconListWidget->count(); i++)
	{
		QListWidgetItem* item = assetIconListWidget->item(i);
		if (item->text().toLower().contains(filterText))
		{
			item->setHidden(false);
		}
		else
		{
			item->setHidden(true);
		}
	}
}

void AssetDock::MeshFileClicked(const std::string meshFilename)
{
	//Set spawner system as MeshActor
	WorldEditor::SetSpawnSystem((IActorSystem*)&MeshActor::system);
	WorldEditor::SetActorTemplateFilename("");

	MeshActor::spawnMeshFilename = meshFilename;
	Log("Mesh [%s] selected in editor.", meshFilename.c_str());
}

void AssetDock::ActorTemplateFileClicked(const std::string actorTemplateFilename)
{
	std::string path = AssetBaseFolders::actorTemplate + actorTemplateFilename;
	Deserialiser d(path, OpenMode::In);

	std::wstring actorSystemName;
	d.is >> actorSystemName;

	IActorSystem* actorSystem = ActorSystemCache::Get().GetSystem(VString::wstos(actorSystemName));

	WorldEditor::SetSpawnSystem(actorSystem);
	Log("[%s] set as spawn system", actorSystem->GetName().c_str());
	WorldEditor::SetActorTemplateFilename(actorTemplateFilename);
}

void AssetDock::TextureFileClicked(const std::wstring textureFilename)
{
	TextureSystem::selectedTextureInEditor = textureFilename;
	Log("[%S] texture selected in editor.", textureFilename.c_str());
}

void AssetDock::MaterialFileClicked(const std::string materialFilename)
{
	MaterialSystem::selectedMaterialInEditor = materialFilename;
	Log("[%s] material selected in editor.", materialFilename.c_str());
}

void AssetDock::CreateNewMapFile()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::AnyFile);

	QString mapFile = dialog.getSaveFileName(nullptr,
		"Create New Map File",
		QString::fromStdString(AssetBaseFolders::worldMap),
		QString::fromStdString(AssetFileExtensions::worldMap),
		nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	//Cancel clicked
	if (mapFile.isEmpty())
	{
		return;
	}

	QFile file(mapFile);
	file.open(QIODevice::WriteOnly);
	file.close();

	//refresh asset items in dock
	AssetFolderClicked();

	//Just load the map when creating it
	QFileInfo fileInfo(file);
	FileSystem::LoadWorld(fileInfo.fileName().toStdString());
}

void AssetDock::CreateNewDialogueFile()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::AnyFile);

	QString dialogueFile = dialog.getSaveFileName(nullptr,
		"Create New Dialogue File",
		QString::fromStdString(AssetBaseFolders::dialogue),
		QString::fromStdString(AssetFileExtensions::dialogue));

	QFile file(dialogueFile);
	file.open(QIODevice::WriteOnly);
	file.close();

	//refresh asset items in dock
	AssetFolderClicked();
}

void AssetDock::CreateNewActorTemplateFile()
{
	if (WorldEditor::GetPickedActor())
	{
		QFileDialog dialog;
		QString actorTemplateFileName = dialog.getSaveFileName(nullptr, "Create New Actor Template File",
			QString::fromStdString(AssetBaseFolders::actorTemplate),
			QString::fromStdString(AssetFileExtensions::actorTemplate));

		if (actorTemplateFileName.isEmpty()) return;

		Serialiser s(actorTemplateFileName.toStdString(), OpenMode::Out);

		//Write out actor's linked actorsystem name
		s.WriteLine(WorldEditor::GetPickedActor()->GetActorSystem()->GetName().c_str());

		//Serialise all actor and actor's component properties
		auto pickedActorProps = WorldEditor::GetPickedActor()->GetProps();
		s.Serialise(pickedActorProps);
		s.WriteLine(L"next");

		for (auto component : WorldEditor::GetPickedActor()->GetAllComponents())
		{
			//Write each component's linked componentsystem name
			s.WriteLine(component->GetTypeName().c_str());

			auto componentProps = component->GetProps();
			s.Serialise(componentProps);

			s.WriteLine(L"next");
		}

		s.WriteLine(L"end");

		QFileInfo fileInfo = actorTemplateFileName;
		Log("Actor Template [%s] created.", fileInfo.fileName().toStdString().c_str());
	}
	else
	{
		Log("No selected actor to create template from.");
	}
}

void AssetDock::CreateNewMaterialFile()
{
	//Create empty material
	auto material = Material("test.png", "Default");
	SerialiseMaterialPropsToFile(material);
}

void AssetDock::CopyMaterialToMaterialFileFromSelectedActor()
{
	Actor* pickedActor = WorldEditor::GetPickedActor();
	if (pickedActor == nullptr)
	{
		Log("No picked actor to create material file with.");
		return;
	}

	MeshComponent* mesh = pickedActor->GetFirstComponentOfTypeAllowNull<MeshComponent>();
	if (mesh == nullptr)
	{
		Log("No mesh on picked actor to create material file with.");
		return;
	}

	SerialiseMaterialPropsToFile(mesh->GetMaterial());
}

void AssetDock::SerialiseMaterialPropsToFile(Material& material)
{
	QString materialFileName = QFileDialog::getSaveFileName(nullptr, "Create Material File",
		QString::fromStdString(AssetBaseFolders::material),
		QString::fromStdString(AssetFileExtensions::material));

	if (materialFileName.isEmpty()) return;

	Serialiser s(materialFileName.toStdString(), OpenMode::Out);
	Properties materialProps = material.GetProps();
	s.Serialise(materialProps);

	Log("Material [%s] created.", materialFileName.toStdString().c_str());
}

void AssetDock::ImportMesh()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::AnyFile);

	const QString filePath = dialog.getOpenFileName(
		nullptr,
		"Import Mesh",
		QString::fromStdString(AssetBaseFolders::fbxFiles),
		nullptr,
		nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	if (!filePath.isEmpty())
	{
		const std::string filename = QFileInfo(filePath).fileName().toStdString();
		AssetSystem::BuildSingleVMeshFromFBX(filePath.toStdString(), filename);
		Log("VMesh built from [%s].", filename.c_str());

		Renderer::SetRendererToCaptureMeshIcon(filename);

		editor->RefreshAssetList();
	}
}

void AssetDock::ImportAnim()
{
	QFileDialog dialog;
	dialog.setFileMode(QFileDialog::AnyFile);

	const QString filePath = dialog.getOpenFileName(
		nullptr,
		"Import Anim",
		QString::fromStdString(AssetBaseFolders::animationFBXFiles),
		nullptr,
		nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	if (!filePath.isEmpty())
	{
		const std::string filename = QFileInfo(filePath).fileName().toStdString();
		AssetSystem::BuildSingleVAnimFromFBX(filePath.toStdString(), filename);
		Log("VAnim built from [%s].", filename.c_str());

		editor->RefreshAssetList();
	}
}

void AssetDock::SetDirectoriesToFilter()
{
	//Only show folders. Don't show parent folders (../ and ./)
	fileSystemModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
}
