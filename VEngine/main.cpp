#include "EditorMainWindow.h"
//#include <QtWidgets/EditorMainWindow>
#include <qpushbutton.h>
#include <QWidget>
#include <qmenubar.h>
#include <qpalette.h>
#include <qtoolbar.h>
#include <QVBoxLayout>
#include <qlistwidget.h>
#include <qlineedit.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtablewidget.h>
#include <qsplitter.h>
#include <QTextBlock>
#include <qgroupbox.h>
#include <qdockwidget.h>
#include <qfilesystemmodel.h>
#include <qcompleter.h>
#include <qtimer.h>
#include <QAbstractNativeEventFilter>
#include <qstylefactory.h>
#include <qtoolbutton.h>


//Old main includes
#include "RenderSystem.h"
#include "CoreSystem.h"
#include "UISystem.h"
#include "Obj.h"
#include "Camera.h"
#include "Audio.h"
#include "AudioSystem.h"
#include "Input.h"
#include "Actor.h"
#include <thread>
#include "ShaderFactory.h"
#include <omp.h>
#include "DebugMenu.h"
#include "Raycast.h"
#include "World.h"
#include "FileSystem.h"
#include "Debug.h"
#include "FBXImporter.h"
#include "WorldEditor.h"
#include "TimerSystem.h"
#include "MathHelpers.h"
#include "Console.h"

struct D3DWidget : public QWidget
{
    ID3D11Device* dev;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapchain;
    ID3D11RenderTargetView* rtv;

    D3DWidget(QWidget* parent = 0);

    void CreateDeviceSwap()
    {
        D3D_FEATURE_LEVEL featureLevels[]
        {
            D3D_FEATURE_LEVEL_11_0
        };
        D3D_FEATURE_LEVEL featureLevel;

        DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferDesc.Width = this->width();
        sd.BufferDesc.Height = this->height();
        sd.BufferDesc.RefreshRate = { 60, 1 };
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferCount = 2;
        sd.SampleDesc.Count = 1;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.Windowed = true;
        sd.OutputWindow = (HWND)this->winId();
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, featureLevels,
            _countof(featureLevels), D3D11_SDK_VERSION, &dev, &featureLevel, &context);

        IDXGIFactory* factory;
        CreateDXGIFactory(IID_PPV_ARGS(&factory));

        factory->CreateSwapChain(dev, &sd, &swapchain);

        //rtv
        ID3D11Texture2D* backBuffer;
        swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        dev->CreateRenderTargetView(backBuffer, nullptr, &rtv);

        context->OMSetRenderTargets(1, &rtv, nullptr);
    }
};

D3DWidget::D3DWidget(QWidget* parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_NativeWindow);

    // Setting these attributes to our widget and returning null on paintEngine event
    // tells Qt that we'll handle all drawing and updating the widget ourselves.

    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    CreateDeviceSwap();
}

struct TransformEditWidget : public QLineEdit
{
    TransformEditWidget(float val, QWidget* parent = 0);


};

TransformEditWidget::TransformEditWidget(float val, QWidget* parent) : QLineEdit(parent)
{
    setValidator(new QDoubleValidator(-D3D11_FLOAT32_MAX, D3D11_FLOAT32_MAX, 32, parent));
    setText(QString::number(val));
}

class PropertiesWidget : public QWidget
{
public:
    TransformEditWidget* posEditX;
    TransformEditWidget* posEditY;
    TransformEditWidget* posEditZ;

    TransformEditWidget* rotEditX;
    TransformEditWidget* rotEditY;
    TransformEditWidget* rotEditZ;

    TransformEditWidget* scaleEditX;
    TransformEditWidget* scaleEditY;
    TransformEditWidget* scaleEditZ;

    PropertiesWidget(QWidget* parent = 0);

    void Tick();

    void setActorPositionX();
    void setActorPositionY();
    void setActorPositionZ();

    void setActorScaleX();
    void setActorScaleY();
    void setActorScaleZ();

    void setActorRotationX();
    void setActorRotationY();
    void setActorRotationZ();

    virtual void enterEvent(QEvent* event)
    {

    }

    virtual void leaveEvent(QEvent* event)
    {

    }
};

void PropertiesWidget::setActorPositionX()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditX)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.x = posEditX->text().toFloat();
        gWorldEditor.pickedActor->SetPosition(pos);
    }

    clearFocus();
}

void PropertiesWidget::setActorPositionY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditY)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.y = posEditY->text().toFloat();
        gWorldEditor.pickedActor->SetPosition(pos);
    }

    clearFocus();
}

void PropertiesWidget::setActorPositionZ()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && posEditZ)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        pos.z = posEditZ->text().toFloat();
        gWorldEditor.pickedActor->SetPosition(pos);
    }

    clearFocus();
}

void PropertiesWidget::setActorScaleX()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditX)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.x = posEditX->text().toFloat();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::setActorScaleY()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditY)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.y = posEditY->text().toFloat();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::setActorScaleZ()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (picked && scaleEditZ)
    {
        XMFLOAT3 scale = picked->GetScale();
        scale.z = posEditZ->text().toFloat();
        gWorldEditor.pickedActor->SetScale(scale);
    }
}

void PropertiesWidget::setActorRotationX()
{
}

void PropertiesWidget::setActorRotationY()
{
}

void PropertiesWidget::setActorRotationZ()
{
}

PropertiesWidget::PropertiesWidget(QWidget* parent) : QWidget(parent)
{
    setFocusPolicy(Qt::FocusPolicy::StrongFocus);

    QGridLayout* grid = new QGridLayout();

    //Position
    QLabel* posLabel = new QLabel("Position");

    posEditX = new TransformEditWidget(0.f, this);
    connect(posEditX, &QLineEdit::returnPressed, this, &PropertiesWidget::setActorPositionX);
    posEditY = new TransformEditWidget(0.f, this);
    connect(posEditY, &QLineEdit::returnPressed, this, &PropertiesWidget::setActorPositionY);
    posEditZ = new TransformEditWidget(0.f, this);
    connect(posEditZ, &QLineEdit::returnPressed, this, &PropertiesWidget::setActorPositionZ);

    grid->addWidget(posLabel, 0, 0);
    grid->addWidget(posEditX, 0, 1);
    grid->addWidget(posEditY, 0, 2);
    grid->addWidget(posEditZ, 0, 3);

    //Rotation
    rotEditX = new TransformEditWidget(0.0f, this);
    rotEditY = new TransformEditWidget(0.0f, this);
    rotEditZ = new TransformEditWidget(0.0f, this);

    QLabel* rotLabel = new QLabel("Rotation");

    grid->addWidget(rotLabel, 1, 0);
    grid->addWidget(rotEditX, 1, 1);
    grid->addWidget(rotEditY, 1, 2);
    grid->addWidget(rotEditZ, 1, 3);

    //Scale
    scaleEditX = new TransformEditWidget(0.0f, this);
    connect(scaleEditX, &QLineEdit::returnPressed, this, &PropertiesWidget::setActorScaleX);
    scaleEditY = new TransformEditWidget(0.0f, this);
    connect(scaleEditY, &QLineEdit::returnPressed, this, &PropertiesWidget::setActorScaleY);
    scaleEditZ = new TransformEditWidget(0.0f, this);
    connect(scaleEditZ, &QLineEdit::returnPressed, this, &PropertiesWidget::setActorScaleZ);


    QLabel* scaleLabel = new QLabel("Scale");

    grid->addWidget(scaleLabel, 2, 0);
    grid->addWidget(scaleEditX, 2, 1);
    grid->addWidget(scaleEditY, 2, 2);
    grid->addWidget(scaleEditZ, 2, 3);

    grid->setAlignment(Qt::AlignTop);

    QHBoxLayout* hLayoutTop = new QHBoxLayout();

    hLayoutTop->addLayout(grid);

    setLayout(hLayoutTop);
}

void PropertiesWidget::Tick()
{
    Actor* picked = gWorldEditor.pickedActor;
    if (posEditX->hasFocus())
    {
        return;
    }
    else if (picked)
    {
        XMFLOAT3 pos = picked->GetPositionFloat3();
        posEditX->setText(QString::number(pos.x));
        posEditY->setText(QString::number(pos.y));
        posEditZ->setText(QString::number(pos.z));

        XMFLOAT3 rot = picked->GetRollPitchYaw();
        rotEditX->setText(QString::number(rot.x));
        rotEditY->setText(QString::number(rot.y));
        rotEditZ->setText(QString::number(rot.z));

        XMFLOAT3 scale = picked->GetScale();
        scaleEditX->setText(QString::number(scale.x));
        scaleEditY->setText(QString::number(scale.y));
        scaleEditZ->setText(QString::number(scale.z));
    }
}

struct MainWidget : public QWidget
{
    MainWidget(QWidget* parent = 0);

    void mousePressEvent(QMouseEvent* mouseEvent) override
    {
        if (mouseEvent->button() == Qt::MouseButton::LeftButton)
        {
            gInputSystem.StoreMouseLeftDownInput(0);
        }
    }

    D3DWidget* d;
};

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{
    
}

struct WorldWidget : QWidget
{
public:
    void Tick();
    void ClickOnListActor(QListWidgetItem* listItem);
    WorldWidget(QWidget* parent = 0);
    void searchWorldList();

    QListWidget* worldList;
    QLineEdit* worldSearch;
    QStringList worldStringList;
};

void WorldWidget::Tick()
{
    worldList->clear();

    World* world = GetWorld();
    for (int i = 0; i < world->actorSystems.size(); i++)
    {
        ActorSystem* ac = world->actorSystems[i];

        for (int j = 0; j < ac->actors.size(); j++)
        {
            QString name = QString::fromStdWString(ac->name) + QString::number(j);
            worldList->addItem(name);
        }
    }
}

void WorldWidget::ClickOnListActor(QListWidgetItem* listItem)
{
    Actor* clickedActor = gWorldEditor.pickedActor;
    if (clickedActor)
    {
        editorCamera.ZoomTo(clickedActor);
    }
}

WorldWidget::WorldWidget(QWidget* parent) : QWidget(parent)
{
    worldList = new QListWidget();
    connect(worldList, SIGNAL(&QListWidget::itemClicked(QListWidgetItem)), this, SLOT(WorldWidget::ClickOnListActor(QListWidgetItem*)));

    //Populate list first time
    World* world = GetWorld();
    for (int i = 0; i < world->actorSystems.size(); i++)
    {
        ActorSystem* ac = world->actorSystems[i];

        for (int j = 0; j < ac->actors.size(); j++)
        {
            QString name = QString::fromStdWString(ac->name) + QString::number(j);
            worldList->addItem(name);
        }
    }

    worldSearch = new QLineEdit();
    worldSearch->setPlaceholderText("Search");
    connect(worldSearch, &QLineEdit::textChanged, this, &WorldWidget::searchWorldList);

    QVBoxLayout* worldVLayout = new QVBoxLayout();
    worldVLayout->addWidget(worldSearch);
    worldVLayout->addWidget(worldList);

    setLayout(worldVLayout);
}

void WorldWidget::searchWorldList()
{
    QString searchText = "*" + worldSearch->text() + "*"; //Wildcards include items with text at any point
    //worldStringList = directory.entryList(QStringList(searchText), QDir::Files);

    worldList->clear();
    worldList->addItems(worldStringList);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorMainWindow app;
    app.setWindowState(Qt::WindowMaximized);

    app.show();

    //app.resize(1024, 720);
    app.setWindowTitle("Testing");

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
    app.addDockWidget(Qt::TopDockWidgetArea, toolbarDock);

    //World list
    WorldWidget* worldWidget = new WorldWidget();

    QDockWidget* levelLayoutDock = new QDockWidget("World");
    levelLayoutDock->setWidget(worldWidget);
    app.addDockWidget(Qt::LeftDockWidgetArea, levelLayoutDock);

    //Properties dock
    QDockWidget* propertiesDock = new QDockWidget("Properties");
    app.addDockWidget(Qt::RightDockWidgetArea, propertiesDock);

    
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
    app.addDockWidget(Qt::BottomDockWidgetArea, assetDock);


    QFileSystemModel* fileModel = new QFileSystemModel();
    fileModel->setRootPath(QDir::currentPath());

    //Console dock

    //Central widget
    MainWidget w;
    app.setCentralWidget(&w);
    app.centralWidget()->setFixedSize(QSize(800, 600));
    QSize size = app.centralWidget()->size();
    gCoreSystem.windowWidth = size.width();
    gCoreSystem.windowHeight = size.height();

    //Properties dock
    PropertiesWidget* propWidget = new PropertiesWidget();
    propertiesDock->setWidget(propWidget);

    //old main
    FBXImporter::Init();

    //gCoreSystem.SetupWindow(GetModuleHandle(NULL), 1);
    gCoreSystem.windowWidth = w.width();
    gCoreSystem.windowHeight = w.height();
    gCoreSystem.SetTimerFrequency();
    gRenderSystem->Init((HWND)w.winId());
    gAudioSystem.Init();
    gUISystem.Init();
    gWorldEditor.Init();

    //TODO: This is causing a crash in Release. Is it because it's after the init code?
    //ID3D11Buffer* debugLinesBuffer = gRenderSystem.CreateDefaultBuffer(sizeof(Vertex) * 1024, D3D11_BIND_VERTEX_BUFFER, debugLineData);

    ActorSystem cubes;
    cubes.modelName = "cube.fbx";
    cubes.CreateActors(gRenderSystem, 2);

    World* world = GetWorld();
    world->actorSystems.push_back(&cubes);
    world->actorSystems.push_back(&gWorldEditor.xAxis);
    world->actorSystems.push_back(&gWorldEditor.yAxis);
    world->actorSystems.push_back(&gWorldEditor.zAxis);

    worldWidget->Tick();


    //MAIN LOOP
    while (gCoreSystem.bMainLoop)
    {
        const float deltaTime = gCoreSystem.deltaTime;

        a.processEvents();

        //QT ticks testing
        propWidget->Tick();

        gCoreSystem.StartTimer();
        gCoreSystem.HandleMessages();

        gFileSystem.Tick();
        gUISystem.Tick();

        QPoint p = w.mapFromGlobal(QCursor::pos());
        gUISystem.mousePos.x = p.x();
        gUISystem.mousePos.y = p.y();

        editorCamera.Tick(deltaTime);
        gTimerSystem.Tick(deltaTime);

        //test ortho work
        if (gInputSystem.GetKeyDownState('P'))
        {
            gRenderSystem.matrices.proj = XMMatrixOrthographicOffCenterLH(-5.f, 5.f, -5.f, 5.f, -50.f, 1000.f);
        }

        //ACTOR RENDERING
        gRenderSystem.Tick();
        gRenderSystem.RenderSetup(deltaTime);

        gWorldEditor.Tick(nullptr);

        gRenderSystem.RenderActorSystem(world);
        gRenderSystem.RenderBounds();
        gRenderSystem.RenderEnd(deltaTime, nullptr);

        

        //UI RENDERING
        if (gUISystem.bAllUIActive)
        {
            gUISystem.d2dRenderTarget->BeginDraw();
            gConsole.Tick();
            gConsole.DrawViewItems();
            //debugMenu.Tick(GetWorld(), deltaTime);
            gUISystem.RenderAllUIViews();
            gUISystem.d2dRenderTarget->EndDraw();
        }

        //PRESENT
        HR(gRenderSystem.swapchain->Present(1, 0));

        gInputSystem.InputReset();

        gCoreSystem.EndTimer();
    }

    gUISystem.Cleanup();
    qApp->quit();

    return 0;
}


