#include "EditorMainWindow.h"
#include <qboxlayout.h>
#include <qtoolbutton.h>
#include <qdockwidget.h>
#include <qlistwidget.h>
#include <qfilesystemmodel.h>
#include "RenderViewWidget.h"
#include "PropertiesWidget.h"
#include "CoreSystem.h"
#include <QTextEdit>
#include "ToolbarDock.h"
#include "WorldDock.h"
#include "PropertiesDock.h"
#include "AssetDock.h"
#include "ConsoleDock.h"
#include <qapplication.h>
#include <QLabel>

//Because I can't figure out why Qt doesn't like these two not being defined in main, just going with
//two pointers here to those instances defined in main.

void EditorMainWindow::Init(int argc)
{
    gQApplication = new QApplication(argc, nullptr);
}

EditorMainWindow::EditorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    //Menu for recreating closed tabs and docks
    QMenu* windowMenu = menuBar()->addMenu("Window");
    auto console = new QAction("Console");
    connect(console, &QAction::triggered, this, &EditorMainWindow::CreateConsoleDock);
    windowMenu->addAction(console);

    //WORLD EDITOR TAB
    editorTabWidget = new QTabWidget();
    connect(editorTabWidget, &QTabWidget::tabCloseRequested, this, &EditorMainWindow::CloseTab);
    setCentralWidget(editorTabWidget);
    editorTabWidget->setTabsClosable(true);

    worldEditorWindow = new QMainWindow();
    editorTabWidget->addTab(worldEditorWindow, "World Editor");

    //Toolbar dock
    toolbarDock = new ToolbarDock("Toolbar");
    worldEditorWindow->addDockWidget(Qt::TopDockWidgetArea, toolbarDock);

    //World list
    worldDock = new WorldDock("World");
    worldEditorWindow->addDockWidget(Qt::LeftDockWidgetArea, worldDock);

    //Properties dock
    propertiesDock = new PropertiesDock("Properties");
    worldEditorWindow->addDockWidget(Qt::RightDockWidgetArea, propertiesDock);

    //Asset dock
    assetDock = new AssetDock("Assets");
    worldEditorWindow->addDockWidget(Qt::BottomDockWidgetArea, assetDock);

    //Console dock
    consoleDock = new ConsoleDock();
    worldEditorWindow->addDockWidget(Qt::BottomDockWidgetArea, consoleDock);
    //connect(consoleDock, &QDockWidget::close, this, &EditorMainWindow::CloseConsoleDock);

    //Central widget - Viewport
    {
        renderViewWidget = new RenderViewWidget();
        worldEditorWindow->setCentralWidget(renderViewWidget);
        //centralWidget()->setFixedSize(QSize(1000, 600));
        QSize size = renderViewWidget->size();
        gCoreSystem.windowWidth = size.width();
        gCoreSystem.windowHeight = size.height();
    }

    //last setup stage
    setWindowState(Qt::WindowMaximized);
    show();
    setWindowTitle("VEngine");
}

void EditorMainWindow::closeEvent(QCloseEvent* event)
{
    gCoreSystem.bMainLoop = false;
}

bool EditorMainWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
    MSG* msg = (MSG*)message;

    switch (msg->message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);

    case WM_KEYDOWN:
        gInputSystem.StoreKeyDownInput(msg->wParam);

        //Close editor
        if (msg->wParam == VK_ESCAPE)
        {

        }

        break;

    case WM_KEYUP:
        gInputSystem.StoreKeyUpInput(msg->wParam);
        break;

    case WM_LBUTTONUP:
        gInputSystem.StoreMouseLeftUpInput(msg->wParam);
        break;

    case WM_LBUTTONDOWN:
        gInputSystem.StoreMouseLeftDownInput(msg->wParam);
        break;

    case WM_RBUTTONUP:
        gInputSystem.StoreMouseRightUpInput(msg->wParam);
        break;

    case WM_RBUTTONDOWN:
        gInputSystem.StoreMouseRightDownInput(msg->wParam);
        break;

    case WM_MOUSEWHEEL:
        if (GET_WHEEL_DELTA_WPARAM(msg->wParam) < 0)
        {
            gInputSystem.StoreMouseWheelDown();
        }
        else
        {
            gInputSystem.StoreMouseWheelUp();
        }
        break;
    }

    return false;
}

void EditorMainWindow::Tick()
{
    toolbarDock->Tick();
    assetDock->Tick();
    worldDock->Tick();
    propertiesDock->Tick();
    renderViewWidget->Tick();
}

void EditorMainWindow::Print(const char* string)
{
    consoleDock->consoleMessageBox->insertPlainText(QString(string));
    consoleDock->consoleMessageBox->insertPlainText("\n");
}

void EditorMainWindow::CloseTab(int tabIndex)
{
    editorTabWidget->removeTab(tabIndex);
}

void EditorMainWindow::CreateConsoleDock()
{
    if (consoleDock == nullptr)
    {
        consoleDock = new ConsoleDock();
        consoleDock->setFloating(true);
        worldEditorWindow->addDockWidget(Qt::DockWidgetArea::NoDockWidgetArea, consoleDock);
    }
}

void EditorMainWindow::CloseConsoleDock()
{
    delete consoleDock;
    consoleDock = nullptr;
}
