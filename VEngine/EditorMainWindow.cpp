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

//Because I can't figure out why Qt doesn't like these two not being defined in main, just going with
//two pointers here to those instances defined in main.
QApplication* gQApplication;
EditorMainWindow* gEditorMainWindow;

EditorMainWindow::EditorMainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    //Toolbar dock
    toolbarDock = new ToolbarDock("Toolbar");
    addDockWidget(Qt::TopDockWidgetArea, toolbarDock);

    //World list
    worldDock = new WorldDock("World");
    addDockWidget(Qt::LeftDockWidgetArea, worldDock);

    //Properties dock
    propertiesDock = new PropertiesDock("Properties");
    addDockWidget(Qt::RightDockWidgetArea, propertiesDock);

    //Asset dock
    assetDock = new AssetDock("Assets");
    addDockWidget(Qt::BottomDockWidgetArea, assetDock);

    //Console dock
    consoleDock = new ConsoleDock();
    tabifyDockWidget(assetDock, consoleDock); //Adds second argument as tab to first's Dock Area

    //Central widget - Viewport
    {
        renderViewWidget = new RenderViewWidget();
        setCentralWidget(renderViewWidget);
        centralWidget()->setFixedSize(QSize(1000, 600));
        QSize size = centralWidget()->size();
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
