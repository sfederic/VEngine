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

    auto intWidget = new IntWidget((int*)&actorSystem->numVertices, "bigcockintbox");

    auto grid = new QGridLayout();
    grid->setColumnMinimumWidth(0, 100);
    grid->setColumnMinimumWidth(1, 100);

    for (int i = 0; i < 5; i++)
    {
        grid->addWidget(new QLabel("Position"), i, 0);
        grid->addWidget(intWidget, i, 1);
    }

    widget->setLayout(grid);

    setWidget(widget);
}
