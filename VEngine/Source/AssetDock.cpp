#include "AssetDock.h"
#include <qdiriterator.h>
#include <qlistwidget.h>
#include <qlayout.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>
#include <QModelIndexList>
#include <QModelIndex>
#include <QDirModel>
#include <qdesktopservices.h>
#include <qurl.h>
#include <filesystem>
#include "World.h"
#include "FileSystem.h"
#include <qmenu.h>
#include <fstream>
#include <qinputdialog.h>

AssetDock::AssetDock(const char* title) : QDockWidget(title)
{
    fileSystemModel = new QFileSystemModel();
    fileSystemModel->setRootPath(QDir::currentPath());

    //Only show folders. NoDotAndDotDot are something Qt throws in for parent folders.
    fileSystemModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    assetTreeView = new QTreeView();
    assetTreeView->setMaximumSize(QSize(300, 1000));
    assetTreeView->setModel(fileSystemModel);
    assetTreeView->setRootIndex(fileSystemModel->index(QDir::currentPath()));
    connect(assetTreeView, &QTreeView::clicked, this, &AssetDock::AssetFolderClicked);

    //Hide all columsn except for 'Name' (Other columns mirror Win32 usuals. Date, Type, etc.)
    //Start on i = 1 as 'Name' is first column.
    for (int i = 1; i < fileSystemModel->columnCount(); i++)
    {
        assetTreeView->hideColumn(i);
    }

    assetIcons = new QListWidget();
    assetIcons->setIconSize(QSize(75, 75));
    assetIcons->setViewMode(QListView::ViewMode::IconMode);
    connect(assetIcons, &QListWidget::doubleClicked, this, &AssetDock::AssetItemClicked);

    QHBoxLayout* assetHBox = new QHBoxLayout();
    assetHBox->addWidget(assetTreeView, Qt::AlignLeft);
    assetHBox->addWidget(assetIcons, Qt::AlignRight);

    QWidget* assetWidget = new QWidget();
    assetWidget->setLayout(assetHBox);

    setWidget(assetWidget);

    QFileSystemModel* fileModel = new QFileSystemModel();
    fileModel->setRootPath(QDir::currentPath());


    //Setup context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &AssetDock::ShowCreateAssetContextMenu);
}

void AssetDock::Tick()
{

}


void AssetDock::AssetItemClicked()
{
    QModelIndex index = assetTreeView->currentIndex();
    QString path = fileSystemModel->filePath(index);

    QString assetName = assetIcons->currentItem()->text();
    QString fullPath = path + "/" + assetName;

    //Do whatever based on extension.
    auto fileExtension = std::filesystem::path(fullPath.toStdString()).extension();
    auto extension = fileExtension.c_str();

    //Load world
    if (std::wcscmp(extension, L".sav") == 0)
    {
        gFileSystem.LoadWorld(fullPath.toStdString().c_str());
    }
    else if (std::wcscmp(extension, L".ast") == 0)
    {
        std::filebuf fb;
        fb.open(fullPath.toUtf8(), std::ios_base::in);
        std::istream is(&fb);

        auto templateActorSystem = new ActorSystem();
        templateActorSystem->DeserialiseAsTemplate(is);
        GetWorld()->AddActorSystem(templateActorSystem);
            
    }
    else
    {
        //Opens up default system program from filename.
        QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath));
    }
}

void AssetDock::AssetFolderClicked()
{
    QModelIndex index = assetTreeView->currentIndex();
    QString path = fileSystemModel->filePath(index);

    QDir directory(path);
    QStringList list = directory.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);

    assetIcons->clear();

    QPixmap iconImage = QPixmap("Editor/Icons/test.png");
    QIcon icon = QIcon(iconImage);

    for (int i = 0; i < list.count(); i++)
    {
        QListWidgetItem* item = new QListWidgetItem(icon, list[i]);
        item->setSizeHint(QSize(100, 100));
        assetIcons->addItem(item);
    }
}

void AssetDock::ShowCreateAssetContextMenu(const QPoint& pos)
{
    QMenu contextMenu(tr("Create Asset"), this);

    contextMenu.addAction("New Level", this, &AssetDock::CreateLevel);
    contextMenu.addAction("New Shader", this, &AssetDock::CreateShader);
    contextMenu.addAction("New Material", this, &AssetDock::CreateMaterial);

    contextMenu.exec(mapToGlobal(pos));
}

void AssetDock::CreateLevel()
{
    QString levelName = QInputDialog::getText(this, "New Level", "Enter level name:");
    std::string levelPath = "LevelSaves/" + levelName.toStdString() + ".sav";

    std::ofstream stream(levelPath);
    stream << "" << std::endl;
    stream.close();

    AssetFolderClicked(); //Refresh the asset widget
}

void AssetDock::CreateShader()
{
    QString shaderName = QInputDialog::getText(this, "New Shader", "Enter shader name:");
    std::string levelPath = "Shaders/" + shaderName.toStdString() + ".hlsl";

    std::ofstream stream(levelPath);
    stream << "#include \"Include/CommonTypes.hlsl\"\n\n";
    stream << "VS_OUT VSMain(VS_IN i)\n";
    stream << "{\n\n";
    stream << "}\n\n";
    stream << "float4 PSMain(VS_OUT i) : SV_Target\n";
    stream << "{\n\n";
    stream << "}\n";
    stream.close();

    AssetFolderClicked(); //Refresh the asset widget
}

void AssetDock::CreateMaterial()
{
    QString materialName = QInputDialog::getText(this, "New Material", "Enter material name:");
    std::string materialPath = "Materials/" + materialName.toStdString() + ".vmat";

    std::ofstream stream(materialPath);
    stream << "" << std::endl;
    stream.close();

    AssetFolderClicked();
}
