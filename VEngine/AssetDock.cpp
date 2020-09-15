#include "AssetDock.h"
#include <qdiriterator.h>
#include <qlistwidget.h>
#include <qlayout.h>
#include <qfilesystemmodel.h>

AssetDock::AssetDock(const char* title) : QDockWidget(title)
{
    //Asset dock
    QDirIterator assetDirectory(QDir::currentPath(), QDir::Dirs);

    QListWidget* assetList = new QListWidget();
    while (assetDirectory.hasNext())
    {
        assetList->addItem(assetDirectory.next());
    }

    connect(assetList, &QListWidget::itemClicked, this, &AssetDock::AssetItemClicked);

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
    assetHBox->addWidget(assetList, Qt::AlignLeft);
    assetHBox->addWidget(assetIcons, Qt::AlignRight);

    QWidget* assetWidget = new QWidget();
    assetWidget->setLayout(assetHBox);

    setWidget(assetWidget);

    QFileSystemModel* fileModel = new QFileSystemModel();
    fileModel->setRootPath(QDir::currentPath());
}


void AssetDock::AssetItemClicked(QListWidgetItem* listWidgetItem)
{
    //TODO: set text into asset icon list etc.
     //listWidgetItem->text()
}