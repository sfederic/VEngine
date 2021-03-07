#include "AssetDock.h"
#include <qdiriterator.h>
#include <qlistwidget.h>
#include <qlayout.h>
#include <qfilesystemmodel.h>
#include <qtreeview.h>
#include <QModelIndexList>

QFileSystemModel* fileSystemModel;
QTreeView* assetTreeView;

AssetDock::AssetDock(const char* title) : QDockWidget(title)
{
    fileSystemModel = new QFileSystemModel();
    fileSystemModel->setRootPath(QDir::currentPath());

    assetTreeView = new QTreeView();
    assetTreeView->setModel(fileSystemModel);
    assetTreeView->setRootIndex(fileSystemModel->index(QDir::currentPath()));

    QListWidget* assetIcons = new QListWidget();
    QPixmap iconImage = QPixmap("Editor/Icons/test.png");
    QIcon icon = QIcon(iconImage);
    QListWidgetItem* iconItem = new QListWidgetItem(icon, "testIcon");

    QListWidgetItem* iconItem2 = new QListWidgetItem(icon, "testIcon");

    assetIcons->addItem(iconItem);
    assetIcons->addItem(iconItem2);

    assetIcons->setIconSize(QSize(75, 75));
    assetIcons->setViewMode(QListView::ViewMode::IconMode);

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