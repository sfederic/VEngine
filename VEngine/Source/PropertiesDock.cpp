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

    QWidget* widget = new QWidget();

    //TODO: Better to do something like a grid here (two columns) so you can forloop over the 
    //eventual actorsystemfactory properties per actorsystem.

    auto grid = new QGridLayout();

    int currentGridRow = 0;

    for (auto& actorProperty : actor->GetProperties())
    {
        auto propertyInfo = actorProperty.first;
        auto propertyData = actorProperty.second;

        grid->addWidget(new QLabel(propertyData.first), currentGridRow, 0);

        if (propertyInfo == typeid(bool))
        {
            auto boolWidget = new BoolWidget((bool*)&propertyData.second);
            grid->addWidget(boolWidget, currentGridRow, 1);
        }

        currentGridRow++;
    }

    widget->setLayout(grid);
    setWidget(widget);
}
