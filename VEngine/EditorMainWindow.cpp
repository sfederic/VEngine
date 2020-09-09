#include "EditorMainWindow.h"
#include <qboxlayout.h>
#include <qtoolbutton.h>
#include <qdockwidget.h>
#include <qlistwidget.h>
#include "WorldWidget.h"
#include <qfilesystemmodel.h>
#include "RenderViewWidget.h"
#include "PropertiesWidget.h"
#include "CoreSystem.h"

EditorMainWindow::EditorMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //Toolbar dock
    QHBoxLayout* toolbarHLayout = new QHBoxLayout();

    QToolButton* saveToolButton = new QToolButton();
    QPixmap saveToolPixmap = QPixmap("test.png");
    QIcon saveToolIcon = QIcon(saveToolPixmap);
    saveToolButton->setIcon(saveToolIcon);
    saveToolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    saveToolButton->setText("Save");
    toolbarHLayout->addWidget(saveToolButton, Qt::AlignLeft);

    QToolButton* playToolButton = new QToolButton();
    QPixmap playToolPixmap = QPixmap("play_icon.png");
    QIcon playToolIcon = QIcon(playToolPixmap);
    playToolButton->setIcon(playToolIcon);
    playToolButton->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonTextUnderIcon);
    playToolButton->setText("Play");
    toolbarHLayout->addWidget(playToolButton, Qt::AlignLeft);


    //toolbarHLayout->setSpacing(1);
    toolbarHLayout->addStretch(0);

    QWidget* toolbarWidget = new QWidget();
    toolbarWidget->setLayout(toolbarHLayout);


    QDockWidget* toolbarDock = new QDockWidget("Toolbar");
    toolbarDock->setWidget(toolbarWidget);
    toolbarDock->setMaximumHeight(90);
    addDockWidget(Qt::TopDockWidgetArea, toolbarDock);

    //World list
    WorldWidget* worldWidget = new WorldWidget();

    QDockWidget* levelLayoutDock = new QDockWidget("World");
    levelLayoutDock->setWidget(worldWidget);
    addDockWidget(Qt::LeftDockWidgetArea, levelLayoutDock);

    //Properties dock
    QDockWidget* propertiesDock = new QDockWidget("Properties");
    addDockWidget(Qt::RightDockWidgetArea, propertiesDock);

    //Asset dock
    QListWidget* assetList = new QListWidget();
    assetList->addItem("Thing 1");
    assetList->addItem("Thing 2");
    assetList->addItem("Thing 3");
    assetList->addItem("Thing 4");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");
    assetList->addItem("Thing 5");

    QListWidget* assetIcons = new QListWidget();
    QPixmap iconImage = QPixmap("test.png");
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

    QDockWidget* assetDock = new QDockWidget("Assets");
    assetDock->setWidget(assetWidget);
    addDockWidget(Qt::BottomDockWidgetArea, assetDock);


    QFileSystemModel* fileModel = new QFileSystemModel();
    fileModel->setRootPath(QDir::currentPath());

    //Console dock

    //Central widget
    setCentralWidget(&mainWidget);
    centralWidget()->setFixedSize(QSize(800, 600));
    QSize size = centralWidget()->size();
    gCoreSystem.windowWidth = size.width();
    gCoreSystem.windowHeight = size.height();

    //Properties dock
    propWidget = new PropertiesWidget();
    propertiesDock->setWidget(propWidget);
}
