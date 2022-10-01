#include "vpch.h"
#include "AssetDock.h"
#include <qmenu.h>
#include <qfiledialog.h>
#include <filesystem>
#include <qdockwidget.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>
#include <qlistwidget.h>
#include <QDesktopServices>
#include <qboxlayout.h>
#include <qlineedit.h>
#include <QUrl>
#include "FileSystem.h"
#include "Actors/MeshActor.h"
#include "WorldEditor.h"
#include "PropertiesDock.h"
#include "Render/TextureSystem.h"
#include "Render/Material.h"
#include "Render/ShaderItem.h"
#include "Render/MaterialSystem.h"
#include "Gameplay/GameInstance.h"
#include "Log.h"
#include "Asset/AssetPaths.h"
#include "Asset/AssetFileExtensions.h"
#include "Components/IComponentSystem.h"

std::unordered_map<std::string, std::function<void(QIcon&, std::string&)>> fileExtensionToFunctionMap;

namespace Icons
{
    QPixmap* play;
    QPixmap* font;
    QPixmap* icon;
    QPixmap* lib;
    QPixmap* mesh;
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
    Icons::world = new QPixmap("Icons/world_icon.png");
    Icons::code = new QPixmap("Icons/code_icon.png");
    Icons::material = new QPixmap("Icons/material_icon.png");
    Icons::dialogue = new QPixmap("Icons/dialogue_icon.png");
    Icons::audio = new QPixmap("Icons/audio_icon.png");

    fileSystemModel = new QFileSystemModel();
    fileSystemModel->setRootPath(QDir::currentPath());

    //Only show folders. Don't show parent folders (../ and ./)
    fileSystemModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

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

    assetIcons = new QListWidget();
    assetIcons->setIconSize(QSize(75, 75));
    assetIcons->setViewMode(QListView::ViewMode::IconMode);
    connect(assetIcons, &QListWidget::clicked, this, &AssetDock::AssetItemClicked);
    connect(assetIcons, &QListWidget::doubleClicked, this, &AssetDock::OpenAssetItemInDefaultProgram);

    QHBoxLayout* assetHBox = new QHBoxLayout();
    assetHBox->addWidget(assetTreeView, Qt::AlignLeft);
    assetHBox->addWidget(assetIcons, Qt::AlignRight);


    //Setup search bar
    assetFilterLineEdit = new QLineEdit();
    assetFilterLineEdit->setPlaceholderText("Search Assets...");
    connect(assetFilterLineEdit, &QLineEdit::textChanged, this, &AssetDock::FilterAssets);

    auto vLayout = new QVBoxLayout();
    vLayout->addWidget(assetFilterLineEdit);
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

    QString assetName = assetIcons->currentItem()->text();
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
    else if (std::wcscmp(extension, L".fbx") == 0) //FBX files
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

    QString assetName = assetIcons->currentItem()->text();
    QString fullPath = path + "/" + assetName;

    auto fileExtension = std::filesystem::path(fullPath.toStdString()).extension();
    auto extension = fileExtension.c_str();

    //Opens up default system program from filename.
    QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
}

void AssetDock::AssetFolderClicked()
{
    QModelIndex index = assetTreeView->currentIndex();
    QString folderPath = fileSystemModel->filePath(index);

    QDir directory(folderPath);
    QFileInfoList fileInfoList = directory.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);

    assetIcons->clear();

    for (QFileInfo& fileInfo : fileInfoList)
    {
        QIcon icon;
        std::string filePath = fileInfo.absoluteFilePath().toStdString();
        auto fileExtension = std::filesystem::path(filePath).extension();
        
        auto extToFuncIt = fileExtensionToFunctionMap.find(fileExtension.string());
        if (extToFuncIt != fileExtensionToFunctionMap.end())
        {
            extToFuncIt->second(icon, filePath);
        }
        else if (fileExtension == ".fbx")
        {
            std::string meshIconPath = "Icons/MeshIcons/" + fileInfo.fileName().toStdString() + ".jpg";
            if (std::filesystem::exists(meshIconPath))
            {
                auto pix = QPixmap(meshIconPath.c_str());
                icon = pix.scaled(QSize(720, 512));
            }
            else
            {
                icon = *Icons::mesh;
            }
        }
        else
        {
            //Set Defaut icon
            icon = *Icons::play;
        }

        QListWidgetItem* item = new QListWidgetItem(icon, fileInfo.fileName());
        item->setSizeHint(QSize(150, 100));
        assetIcons->addItem(item);
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

    contextMenu.exec(mapToGlobal(point));
}

void AssetDock::FilterAssets()
{
    QString filterText = assetFilterLineEdit->text().toLower();

    for (int i = 0; i < assetIcons->count(); i++)
    {
        QListWidgetItem* item = assetIcons->item(i);
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
}

void AssetDock::ActorTemplateFileClicked(const std::string actorTemplateFilename)
{
    std::string path = AssetBaseFolders::actorTemplate + actorTemplateFilename;
    Deserialiser d(path, OpenMode::In);

    std::wstring actorSystemName;
    d.is >> actorSystemName;

    IActorSystem* actorSystem = actorSystemCache.Get(VString::wstos(actorSystemName));

    WorldEditor::SetSpawnSystem(actorSystem);
    Log("[%s] set as spawn system", actorSystem->GetName().c_str());
    WorldEditor::SetActorTemplateFilename(actorTemplateFilename);
}

void AssetDock::TextureFileClicked(const std::wstring textureFilename)
{
    textureSystem.selectedTextureInEditor = textureFilename;
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
        QString::fromStdString(AssetFileExtensions::worldMap));

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

        for (auto& componentPair : WorldEditor::GetPickedActor()->componentMap)
        {
            //Write each component's linked componentsystem name
            s.WriteLine(componentPair.second->componentSystem->name.c_str());

            auto componentProps = componentPair.second->GetProps();
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
    auto material = Material("test.png", ShaderItems::Default);

    QFileDialog dialog;
    QString materialFileName = dialog.getSaveFileName(nullptr, "Create Material File",
        QString::fromStdString(AssetBaseFolders::material),
        QString::fromStdString(AssetFileExtensions::material));

    Serialiser s(materialFileName.toStdString(), OpenMode::Out);
    auto materialProps = material.GetProps();
    s.Serialise(materialProps);

    Log("Material [%s] created.", materialFileName.toStdString().c_str());
}
