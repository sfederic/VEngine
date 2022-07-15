#include "vpch.h"
#include "PropertiesDock.h"
#include <qwidget.h>
#include <qboxlayout.h>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QScrollBar>
#include <qscrollarea.h>
#include <QCheckBox>
#include <QLineEdit>
#include "Actors/Actor.h"
#include "Components/Component.h"
#include "Components/IComponentSystem.h"
#include "Components/ComponentSystemCache.h"
#include "Render/PipelineObjects.h"
#include "PropertyWidgets/BoolWidget.h" 
#include "PropertyWidgets/Float2Widget.h"
#include "PropertyWidgets/Float3Widget.h"
#include "PropertyWidgets/Float4Widget.h"
#include "PropertyWidgets/FloatWidget.h"
#include "PropertyWidgets/IntWidget.h"
#include "PropertyWidgets/UintWidget.h"
#include "PropertyWidgets/VectorWidget.h"
#include "PropertyWidgets/StringWidget.h"
#include "PropertyWidgets/WStringWidget.h"
#include "PropertyWidgets/TextureDataWidget.h"
#include "PropertyWidgets/ShaderDataWidget.h"
#include "PropertyWidgets/MeshComponentDataWidget.h"
#include "PropertyWidgets/VEnumWidget.h"
#include "VEnum.h"
#include "Render/Material.h"
#include "Render/RenderTypes.h"

std::unordered_map<std::type_index, std::function<void(Property&, int)>> typeToFunctionMap;

PropertiesDock::PropertiesDock() : QDockWidget("Properties")
{
    setFixedWidth(400);

    actorPropsScrollArea = new QScrollArea();
    actorPropsGridLayout = new QGridLayout();
    actorPropsWidget = new QWidget();
    actorPropsWidget->setFixedWidth(400);

    actorPropsWidget->setLayout(actorPropsGridLayout);
    actorPropsScrollArea->setWidget(actorPropsWidget);
    setWidget(actorPropsScrollArea);

    //Setup map
    typeToFunctionMap[typeid(bool)] = [&](Property& prop, int row) { CreateWidget<bool, BoolWidget> (prop, row); };
    typeToFunctionMap[typeid(int)] = [&](Property& prop, int row) { CreateWidget<int, IntWidget>(prop, row); };
    typeToFunctionMap[typeid(float)] = [&](Property& prop, int row) { CreateWidget<float, FloatWidget>(prop, row); };
    typeToFunctionMap[typeid(XMFLOAT3)] = [&](Property& prop, int row) { CreateWidget<XMFLOAT3, Float3Widget>(prop, row); };
    typeToFunctionMap[typeid(XMFLOAT4)] = [&](Property& prop, int row) { CreateWidget<XMFLOAT4, Float4Widget>(prop, row); };
    typeToFunctionMap[typeid(XMFLOAT2)] = [&](Property& prop, int row) { CreateWidget<XMFLOAT2, Float2Widget>(prop, row); };
    typeToFunctionMap[typeid(XMVECTOR)] = [&](Property& prop, int row) { CreateWidget<XMVECTOR, VectorWidget>(prop, row); };
    typeToFunctionMap[typeid(std::string)] = [&](Property& prop, int row) { CreateWidget<std::string, StringWidget>(prop, row); };
    typeToFunctionMap[typeid(std::wstring)] = [&](Property& prop, int row) { CreateWidget<std::wstring, WStringWidget>(prop, row); };
    typeToFunctionMap[typeid(TextureData)] = [&](Property& prop, int row) { CreateWidget<TextureData, TextureDataWidget>(prop, row); };
    typeToFunctionMap[typeid(ShaderData)] = [&](Property& prop, int row) { CreateWidget<ShaderData, ShaderDataWidget>(prop, row); };
    typeToFunctionMap[typeid(MeshComponentData)] = [&](Property& prop, int row) { CreateWidget<MeshComponentData, MeshComponentDataWidget>(prop, row); };
    typeToFunctionMap[typeid(VEnum)] = [&](Property& prop, int row) { CreateWidget<VEnum, VEnumWidget>(prop, row); };
}

void PropertiesDock::DisplayActorProperties(Actor* actor)
{
    //Set the inner values in the Qt widgets instead of remaking all the widgets below
    if (previousActor == actor)
    {
        ResetPropertyWidgetValues();
        return;
    }

    currentDisplayingActor = actor;

    if (actorPropsWidget && actorPropsGridLayout)
    {
        //@Todo: deleting everything here is junk. There has to be a removeWidget() right?

        delete addComponentButton;
        addComponentButton = nullptr;

        delete componentComboBox;
        componentComboBox = nullptr;

        delete actorPropsGridLayout;
        actorPropsGridLayout = nullptr;

        delete actorPropsWidget;
        actorPropsWidget = nullptr;
    }

    propertyWidgetsToUpdate.clear();

    actorPropsWidget = new QWidget();
    actorPropsGridLayout = new QGridLayout();

    actorPropsGridLayout->setAlignment(Qt::AlignTop);

    //Component 'add' widgets
    addComponentButton = new QPushButton("Add", this);
    connect(addComponentButton, &QPushButton::clicked, this, &PropertiesDock::AddComponentButtonClick);

    componentComboBox = new QComboBox(this);
    for (auto& csPair : *componentSystemCache.nameToSystemMap)
    {
        auto cs = csPair.second;
        componentComboBox->addItem(QString::fromStdString(cs->name));
    }

    actorPropsGridLayout->addWidget(addComponentButton, 0, 0, 1, 1);
    actorPropsGridLayout->addWidget(componentComboBox, 0, 1, 1, 1);

    //Go over actor properties
    int gridRow = 1;

    for (auto& props : actor->GetAllProps())
    {
        IterateOverProperties(props, gridRow);
    }

    actorPropsWidget->setLayout(actorPropsGridLayout);
    actorPropsWidget->setFixedWidth(400);

    actorPropsScrollArea->setWidget(actorPropsWidget);

    setWidget(actorPropsScrollArea);

    previousActor = actor;
}

void PropertiesDock::IterateOverProperties(Properties& props, int& currentGridRow)
{
    //Set title to seperate components/actor properties
    auto titleLabel = new QLabel(props.title.c_str(), this);
    titleLabel->setStyleSheet("font-weight: bold");
    actorPropsGridLayout->addWidget(titleLabel, currentGridRow, 0, 1, 2, Qt::AlignCenter);
    currentGridRow++;

    //Properties
    for (auto& propPair : props.propMap)
    {
        const std::string& name = propPair.first;
        Property& prop = propPair.second;

        if (prop.hide)
        {
            continue;
        }

        //Set property name onto label
        actorPropsGridLayout->addWidget(new QLabel(name.c_str()), currentGridRow, propertyNameColumn);

        //Call create widget function based on property type
        auto& createWidgetFunc = typeToFunctionMap[prop.info.value()];
        createWidgetFunc(prop, currentGridRow);

        currentGridRow++;
    }
}

void PropertiesDock::Clear()
{
    previousActor = nullptr;
    propertyWidgetsToUpdate.clear();

    if (actorPropsGridLayout && actorPropsWidget)
    {
        delete actorPropsGridLayout;
        actorPropsGridLayout = nullptr;

        delete actorPropsWidget;
        actorPropsWidget = nullptr;
    }
}

void PropertiesDock::AddComponentButtonClick(bool checked)
{
    QString selectedComponentName = componentComboBox->currentText();
    IComponentSystem* componentSystem = componentSystemCache.Get(selectedComponentName.toStdString());
    componentSystem->SpawnComponent(currentDisplayingActor);

    previousActor = nullptr; //set to null so same actor can update

    DisplayActorProperties(currentDisplayingActor); //Reset actor props display
    ResetPropertyWidgetValues();
    actorPropsScrollArea->verticalScrollBar()->setValue(
        actorPropsScrollArea->verticalScrollBar()->maximum()); //Move scroll bar to its end
}

void PropertiesDock::ResetPropertyWidgetValues()
{
    for (IPropertyWidget* propertyWidget : propertyWidgetsToUpdate)
    {
        propertyWidget->ResetValue();
    }
}
