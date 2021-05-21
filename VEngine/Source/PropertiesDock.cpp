#include "PropertiesDock.h"
#include "PropertiesWidget.h"
#include "World.h"
#include <QLineEdit>
#include "../EditorMainWindow.h"
#include "EditorSystem.h"
#include "PropertyWidgets/BoolWidget.h"
#include "PropertyWidgets/FloatWidget.h"
#include "PropertyWidgets/IntWidget.h"
#include "PropertyWidgets/VectorWidget.h"
#include <QVBoxLayout>
#include <qlabel.h>
#include "ActorSystemFactory.h"
#include <typeindex>
#include <qscrollarea.h>

PropertiesDock::PropertiesDock(const char* title) : QDockWidget(title)
{
    setMinimumWidth(400);

    propWidget = new PropertiesWidget();

    auto scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(propWidget);

    setWidget(scrollArea);
}

void PropertiesDock::enterEvent(QEvent* event)
{
    gEditorSystem->SetDockFocus(EDockFocus::Properties);
}

void PropertiesDock::Tick()
{
    if (gEditorSystem->GetDockFocus() != (int)EDockFocus::Properties)
    {
        propWidget->Tick();
    }
}

//Throw all of the selected actor's variables into the properties dock
void PropertiesDock::DisplayActorSystemProperties(Actor* actor)
{
    //Actor
    propWidget->actorName->setText(QString::fromStdWString(actor->name));

    ActorSystem* actorSystem = actor->GetActorSystem();

    //Actor system 
    propWidget->actorSystemName->setText(QString::fromStdWString(actorSystem->name));
    propWidget->actorSystemModelName->setText(QString::fromStdString(actorSystem->modelName));
    propWidget->actorSystemTextureName->setText(QString::fromStdWString(actorSystem->textureName));
    propWidget->actorSystemShaderName->setText(QString::fromStdWString(actorSystem->shaderName));

    propWidget->selectedActorSystem = actorSystem;

    auto actorPropWidgetIt = actorPropWidgetMap.find(actorSystem);
    if (actorPropWidgetIt == actorPropWidgetMap.end())
    {
        QWidget* actorPropsWidget = new QWidget();

        actorPropWidgetMap[actorSystem] = actorPropsWidget;

        auto grid = new QGridLayout();

        int currentGridRow = 0;
        const int propertyNameColumn = 0;
        const int propertyDataColumn = 1;

        for (auto& actorProperty : actor->GetProperties().dataMap)
        {
            auto typeMap = actor->GetProperties().typeMap;
            auto type = typeMap.find(actorProperty.first);

            //Set property name onto label
            grid->addWidget(new QLabel(actorProperty.first.c_str()), currentGridRow, propertyNameColumn);

            //Set property data into grid
            if (type->second == typeid(bool))
            {
                auto boolWidget = new BoolWidget((bool*)actorProperty.second);
                grid->addWidget(boolWidget, currentGridRow, propertyDataColumn);
            }
            else if (type->second == typeid(int))
            {
                auto intWidget = new IntWidget((int*)actorProperty.second);
                grid->addWidget(intWidget, currentGridRow, propertyDataColumn);
            }
            else if (type->second == typeid(float))
            {
                auto floatWidget = new FloatWidget((float*)actorProperty.second);
                grid->addWidget(floatWidget, currentGridRow, propertyDataColumn);
            }
            else if (type->second == typeid(XMVECTOR))
            {
                auto vectorWidget = new VectorWidget((XMVECTOR*)actorProperty.second);
                grid->addWidget(vectorWidget, currentGridRow, propertyDataColumn);
            }

            currentGridRow++;
        }

        actorPropsWidget->setLayout(grid);

        propWidget->entireVLayout->addWidget(actorPropsWidget);
    }
    else
    {
        //TODO: got to add a bit more here to handle different actorsystem clicks
    } 
}
