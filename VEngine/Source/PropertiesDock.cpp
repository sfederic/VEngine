#include "PropertiesDock.h"
#include "World.h"

PropertiesDock::PropertiesDock(const char* title) : QDockWidget(title)
{
    propWidget = new PropertiesWidget();
    setWidget(propWidget);
}

void PropertiesDock::Tick()
{
    propWidget->Tick();
}

//Throw all of the selected actor's variables into the properties dock
void PropertiesDock::DisplayActorSystemProperties(ActorSystem* actorSystem)
{
    propWidget->actorSystemName->setText(QString::fromStdWString(actorSystem->name));
    propWidget->actorSystemModelName->setText(QString::fromStdWString(actorSystem->shaderName));
    propWidget->actorSystemTextureName->setText(QString::fromStdWString(actorSystem->textureName));
    propWidget->actorSystemModelName->setText(QString::fromStdString(actorSystem->modelName));
}
