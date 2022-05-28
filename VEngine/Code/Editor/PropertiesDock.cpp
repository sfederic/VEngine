#include "vpch.h"
#include "PropertiesDock.h"
#include <qwidget.h>
#include <qboxlayout.h>
#include <QDoubleSpinBox>
#include <QLabel>
#include <qscrollarea.h>
#include <QCheckBox>
#include <QLineEdit>
#include "Actors/Actor.h"
#include "Components/Component.h"
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
#include "PropertyWidgets/TransformWidget.h"
#include "PropertyWidgets/VEnumWidget.h"
#include "VEnum.h"
#include "Render/Material.h"
#include "Render/RenderTypes.h"

#include "Profile.h"
#include "Log.h"

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
    typeToFunctionMap[typeid(bool)] = [&](Property& prop, int row) { CreateBoolWidget(prop, row); };
    typeToFunctionMap[typeid(int)] = [&](Property& prop, int row) { CreateIntWidget(prop, row); };
    typeToFunctionMap[typeid(float)] = [&](Property& prop, int row) { CreateFloatWidget(prop, row); };
    typeToFunctionMap[typeid(XMFLOAT3)] = [&](Property& prop, int row) { CreateFloat3Widget(prop, row); };
    typeToFunctionMap[typeid(XMFLOAT4)] = [&](Property& prop, int row) { CreateFloat4Widget(prop, row); };
    typeToFunctionMap[typeid(XMFLOAT2)] = [&](Property& prop, int row) { CreateFloat2Widget(prop, row); };
    typeToFunctionMap[typeid(XMVECTOR)] = [&](Property& prop, int row) { CreateVectorWidget(prop, row); };
    typeToFunctionMap[typeid(std::string)] = [&](Property& prop, int row) { CreateStringWidget(prop, row); };
    typeToFunctionMap[typeid(std::wstring)] = [&](Property& prop, int row) { CreateWStringWidget(prop, row); };
    typeToFunctionMap[typeid(TextureData)] = [&](Property& prop, int row) { CreateTextureDataWidget(prop, row); };
    typeToFunctionMap[typeid(ShaderData)] = [&](Property& prop, int row) { CreateShaderDataWidget(prop, row); };
    typeToFunctionMap[typeid(MeshComponentData)] = [&](Property& prop, int row) { CreateMeshComponentDataWidget(prop, row); };
    typeToFunctionMap[typeid(Transform)] = [&](Property& prop, int row) { CreateTransformWidget(prop, row); };
    typeToFunctionMap[typeid(VEnum)] = [&](Property& prop, int row) { CreateVEnumWidget(prop, row); };
}

void PropertiesDock::DisplayActorProperties(Actor* actor)
{
    auto s = Profile::QuickStart();

    //Set the inner values in the Qt widgets instead of remaking all the widgets below
    if (previousActor == actor)
    {
        ResetPropertyWidgetValues();
        return;
    }

    if (actorPropsWidget && actorPropsGridLayout)
    {
        propertyWidgetsToUpdate.clear();
        delete actorPropsGridLayout;
        delete actorPropsWidget;
    }

    actorPropsWidget = new QWidget();
    actorPropsGridLayout = new QGridLayout();
    actorPropsGridLayout->setAlignment(Qt::AlignTop);

    //Go over actor properties
    int gridRow = 0;

    for (auto& props : actor->GetAllProps())
    {
        IterateOverProperties(props, gridRow);
    }

    actorPropsWidget->setLayout(actorPropsGridLayout);
    actorPropsWidget->setFixedWidth(400);

    actorPropsScrollArea->setWidget(actorPropsWidget);

    setWidget(actorPropsScrollArea);

    previousActor = actor;

    auto e = Profile::QuickEnd(s);
    Log("Prop Time: %f", e);
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

void PropertiesDock::ResetPropertyWidgetValues()
{
    for (IPropertyWidget* propertyWidget : propertyWidgetsToUpdate)
    {
        propertyWidget->ResetValue();
    }
}

void PropertiesDock::CreateBoolWidget(Property& prop, int currentGridRow)
{
    auto boolWidget = new BoolWidget(prop.GetData<bool>());
    actorPropsGridLayout->addWidget(boolWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)boolWidget);
}

void PropertiesDock::CreateIntWidget(Property& prop, int currentGridRow)
{
    auto intWidget = new IntWidget(prop.GetData<int>());
    actorPropsGridLayout->addWidget(intWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)intWidget);
}

void PropertiesDock::CreateFloatWidget(Property& prop, int currentGridRow)
{
    auto floatWidget = new FloatWidget(prop.GetData<float>());
    actorPropsGridLayout->addWidget(floatWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)floatWidget);
}

void PropertiesDock::CreateFloat3Widget(Property& prop, int currentGridRow)
{
    auto float3Widget = new Float3Widget(prop.GetData<XMFLOAT3>());
    actorPropsGridLayout->addWidget(float3Widget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)float3Widget);
}

void PropertiesDock::CreateFloat4Widget(Property& prop, int currentGridRow)
{
    auto float4Widget = new Float4Widget(prop.GetData<XMFLOAT4>());
    actorPropsGridLayout->addWidget(float4Widget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)float4Widget);
}

void PropertiesDock::CreateFloat2Widget(Property& prop, int currentGridRow)
{
    auto float2Widget = new Float2Widget(prop.GetData<XMFLOAT2>());
    actorPropsGridLayout->addWidget(float2Widget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)float2Widget);
}

void PropertiesDock::CreateVectorWidget(Property& prop, int currentGridRow)
{
    auto vectorWidget = new VectorWidget(prop.GetData<XMVECTOR>());
    actorPropsGridLayout->addWidget(vectorWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)vectorWidget);
}

void PropertiesDock::CreateStringWidget(Property& prop, int currentGridRow)
{
    auto stringWidget = new StringWidget(prop);
    actorPropsGridLayout->addWidget(stringWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)stringWidget);
}

void PropertiesDock::CreateWStringWidget(Property& prop, int currentGridRow)
{
    auto wstringWidget = new WStringWidget(prop);
    actorPropsGridLayout->addWidget(wstringWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)wstringWidget);
}

void PropertiesDock::CreateTextureDataWidget(Property& prop, int currentGridRow)
{
    auto textureDataWidget = new TextureDataWidget(prop);
    actorPropsGridLayout->addWidget(textureDataWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)textureDataWidget);
}

void PropertiesDock::CreateShaderDataWidget(Property& prop, int currentGridRow)
{
    auto shaderDataWidget = new ShaderDataWidget(prop);
    actorPropsGridLayout->addWidget(shaderDataWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)shaderDataWidget);
}

void PropertiesDock::CreateMeshComponentDataWidget(Property& prop, int currentGridRow)
{
    auto meshComponentDataWidget = new MeshComponentDataWidget(prop);
    actorPropsGridLayout->addWidget(meshComponentDataWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)meshComponentDataWidget);
}

void PropertiesDock::CreateTransformWidget(Property& prop, int currentGridRow)
{
    auto transformWidget = new TransformWidget(prop.GetData<Transform>(), this);
    actorPropsGridLayout->addWidget(transformWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)transformWidget);
}

void PropertiesDock::CreateVEnumWidget(Property& prop, int currentGridRow)
{
    auto vEnumWidget = new VEnumWidget(prop);
    actorPropsGridLayout->addWidget(vEnumWidget, currentGridRow, propertyDataColumn);
    propertyWidgetsToUpdate.push_back((IPropertyWidget*)vEnumWidget);
}
