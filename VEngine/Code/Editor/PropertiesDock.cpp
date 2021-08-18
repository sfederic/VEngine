#include "PropertiesDock.h"
#include <qwidget.h>
#include <qboxlayout.h>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include "Actors/Actor.h"
#include "PropertyWidgets/BoolWidget.h" 
#include "PropertyWidgets/Float3Widget.h"
#include "PropertyWidgets/FloatWidget.h"
#include "PropertyWidgets/IntWidget.h"
#include "PropertyWidgets/VectorWidget.h"

PropertiesDock::PropertiesDock() : QDockWidget("Properties")
{

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

    int currentGridRow = 0;
    const int propertyNameColumn = 0;
    const int propertyDataColumn = 1;

    Properties props = actor->GetProps();

    for (auto actorProperty : props.dataMap)
    {
        auto typeMap = props.typeMap;
        auto type = typeMap.find(actorProperty.first);

        //Set property name onto label
        actorPropsGridLayout->addWidget(new QLabel(actorProperty.first.c_str()), currentGridRow, propertyNameColumn);

        //Set property data into grid
        if (type->second == typeid(bool))
        {
            auto boolWidget = new BoolWidget((bool*)actorProperty.second);
            actorPropsGridLayout->addWidget(boolWidget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)boolWidget);
        }
        else if (type->second == typeid(int))
        {
            auto intWidget = new IntWidget((int*)actorProperty.second);
            actorPropsGridLayout->addWidget(intWidget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)intWidget);
        }
        else if (type->second == typeid(float))
        {
            //auto floatWidget = new FloatWidget((float*)actorProperty.second);
            //actorPropsGridLayout->addWidget(floatWidget, currentGridRow, propertyDataColumn);
        }
        else if (type->second == typeid(XMFLOAT3))
        {
            auto float3Widget = new Float3Widget((XMFLOAT3*)actorProperty.second);
            actorPropsGridLayout->addWidget(float3Widget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)float3Widget);
        }
        else if (type->second == typeid(XMVECTOR))
        {
            auto vectorWidget = new VectorWidget((XMVECTOR*)actorProperty.second);
            actorPropsGridLayout->addWidget(vectorWidget, currentGridRow, propertyDataColumn);
            propertyWidgetsToUpdate.push_back((IPropertyWidget*)vectorWidget);
        }

        currentGridRow++;
    }

    actorPropsWidget->setLayout(actorPropsGridLayout);
    setWidget(actorPropsWidget);

    previousActor = actor;
}
