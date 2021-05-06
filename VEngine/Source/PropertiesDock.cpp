#include "PropertiesDock.h"
#include "PropertiesWidget.h"
#include "World.h"
#include <QLineEdit>
#include "../EditorMainWindow.h"
#include "EditorSystem.h"
#include "PropertyWidgets/BoolWidget.h"
#include "PropertyWidgets/FloatWidget.h"
#include "PropertyWidgets/IntWidget.h"
#include <QVBoxLayout>
#include <qlabel.h>
#include "ActorSystemFactory.h"
#include <typeindex>

PropertiesDock::PropertiesDock(const char* title) : QDockWidget(title)
{
    setMinimumWidth(400);

    propWidget = new PropertiesWidget();
    setWidget(propWidget);
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


    //TODO: Better to do something like a grid here (two columns) so you can forloop over the 
    //eventual actorsystemfactory properties per actorsystem.
    QWidget* widget = new QWidget();
    auto grid = new QGridLayout();

    int currentGridRow = 0;
    const int propertyNameColumn = 0;
    const int propertyDataColumn = 1;

    for (auto& actorProperty : actor->GetProperties())
    {
        auto propertyInfo = actorProperty.first;
        auto propertyData = actorProperty.second;

        //Set property name onto label
        grid->addWidget(new QLabel(propertyData.first), currentGridRow, propertyNameColumn);

        //Set property data into grid
        if (propertyInfo == typeid(bool))
        {
            auto boolWidget = new BoolWidget((bool*)propertyData.second);
            grid->addWidget(boolWidget, currentGridRow, propertyDataColumn);
        }
        else if (propertyInfo == typeid(int))
        {
            auto intWidget = new IntWidget((int*)propertyData.second);
            grid->addWidget(intWidget, currentGridRow, propertyDataColumn);
        }
        else if (propertyInfo == typeid(float))
        {
            auto floatWidget = new FloatWidget((float*)propertyData.second);
            grid->addWidget(floatWidget, currentGridRow, propertyDataColumn);
        }

        currentGridRow++;
    }

    widget->setLayout(grid);
    setWidget(widget);
}
