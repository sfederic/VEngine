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
