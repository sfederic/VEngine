#pragma once

#include <qdockwidget.h>
#include "PropertiesWidget.h"

class World;

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock(const char* title);
	void Tick();

	PropertiesWidget* propWidget;
};

