#pragma once

#include <qdockwidget.h>
#include "PropertiesWidget.h"

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock(const char* title);

	PropertiesWidget* propWidget;
};

