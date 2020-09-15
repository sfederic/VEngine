#include "PropertiesDock.h"

PropertiesDock::PropertiesDock(const char* title) : QDockWidget(title)
{
    propWidget = new PropertiesWidget();
    setWidget(propWidget);
}
