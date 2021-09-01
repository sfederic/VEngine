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
#include "PropertyWidgets/BoolWidget.h" 
#include "PropertyWidgets/Float3Widget.h"
#include "PropertyWidgets/FloatWidget.h"
#include "PropertyWidgets/IntWidget.h"
#include "PropertyWidgets/VectorWidget.h"
#include "PropertyWidgets/StringWidget.h"

PropertiesDock::PropertiesDock() : QDockWidget("Properties")
{
    actorPropsScrollArea = new QScrollArea(this);
}

void PropertiesDock::DisplayActorProperties(Actor* actor)
{
    //Set the inner values in the Qt widgets instead of remaking all the widgets below
    if (previousActor == actor)
    {
        for (IPropertyWidget* propertyWidget : propertyWidgetsToUpdate)
        {
            propertyWidget->ResetValue();
        }

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
    Properties props = actor->GetProps();
    int gridRow = 0;
    IterateOverProperties(props, gridRow);

    //Go over component properties
    for (Component* component : actor->components)
    {
        Properties componentProps = component->GetProps();
        IterateOverProperties(componentProps, gridRow);
    }

    actorPropsWidget->setLayout(actorPropsGridLayout);
    setWidget(actorPropsWidget);

    actorPropsScrollArea->setWidget(actorPropsWidget);

    setWidget(actorPropsScrollArea);

    previousActor = actor;
}

void PropertiesDock::IterateOverProperties(Properties& props, int& currentGridRow)
{
    const int propertyNameColumn = 0;
    const int propertyDataColumn = 1;

    //Set title to seperate components/actor properties
    auto titleLabel = new QLabel(props.title.c_str(), this);
    titleLabel->setStyleSheet("font-weight: bold");
    actorPropsGridLayout->addWidget(titleLabel, currentGridRow, 0, 1, 2, Qt::AlignCenter);
    currentGridRow++;

    //Properties
    for (auto prop : props.propMap)
    {
        const std::string& name = prop.first;

        //Set property name onto label
        actorPropsGridLayout->addWidget(new QLabel(name.c_str()), currentGridRow, propertyNameColumn);

        //Set property data into grid
        if (props.CheckType<bool>(name))
        {
            auto boolWidget = new BoolWidget(props.GetData<bool>(name));
            actorPropsGridLayout->addWidget(boolWidget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)boolWidget);
        }
        else if (props.CheckType<int>(name))
        {
            auto intWidget = new IntWidget(props.GetData<int>(name));
            actorPropsGridLayout->addWidget(intWidget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)intWidget);
        }
        else if (props.CheckType<float>(name))
        {
            //auto floatWidget = new FloatWidget((float*)actorProperty.second);
            //actorPropsGridLayout->addWidget(floatWidget, currentGridRow, propertyDataColumn);
        }
        else if (props.CheckType<XMFLOAT3>(name))
        {
            auto float3Widget = new Float3Widget(props.GetData<XMFLOAT3>(name));
            actorPropsGridLayout->addWidget(float3Widget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)float3Widget);
        }
        else if (props.CheckType<XMVECTOR>(name))
        {
            auto vectorWidget = new VectorWidget(props.GetData<XMVECTOR>(name));
            actorPropsGridLayout->addWidget(vectorWidget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)vectorWidget);
        }
        else if (props.CheckType<std::string>(name))
        {
            auto stringWidget = new StringWidget(props.GetData<std::string>(name));
            actorPropsGridLayout->addWidget(stringWidget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)stringWidget);
        }

        currentGridRow++;
    }
}
