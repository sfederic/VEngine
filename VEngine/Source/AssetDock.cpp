#include "AssetDock.h"
#include <qdiriterator.h>
#include <qlistwidget.h>
#include <qlayout.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>
#include <QModelIndexList>
#include <QModelIndex>
#include <QDirModel>

AssetDock::AssetDock(const char* title) : QDockWidget(title)
{
    fileSystemModel = new QFileSystemModel();
    fileSystemModel->setRootPath(QDir::currentPath());

    //Only show folders. NoDotAndDotDot are something Qt throws in for parent folders.
    fileSystemModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    assetTreeView = new QTreeView();
    assetTreeView->setModel(fileSystemModel);
    assetTreeView->setRootIndex(fileSystemModel->index(QDir::currentPath()));
    connect(assetTreeView, &QTreeView::clicked, this, &AssetDock::AssetFolderClicked);

    assetIcons = new QListWidget();
    assetIcons->setIconSize(QSize(75, 75));
    assetIcons->setViewMode(QListView::ViewMode::IconMode);
    connect(assetIcons, &QListWidget::clicked, this, &AssetDock::AssetItemClicked);

    QHBoxLayout* assetHBox = new QHBoxLayout();
    assetHBox->addWidget(assetTreeView, Qt::AlignLeft);
    assetHBox->addWidget(assetIcons, Qt::AlignRight);

    QWidget* assetWidget = new QWidget();
    assetWidget->setLayout(assetHBox);

    setWidget(assetWidget);

    QFileSystemModel* fileModel = new QFileSystemModel();
    fileModel->setRootPath(QDir::currentPath());
}

void AssetDock::Tick()
{
    modelIndex = &assetTreeView->currentIndex();
    if (modelIndex->data().toString() == "main.cpp")
    {
        throw;
    }
}


void AssetDock::AssetItemClicked(QListWidgetItem* listWidgetItem)
{
     //listWidgetItem->text()
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