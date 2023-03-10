#pragma once

#include <qdockwidget.h>
#include <qgridlayout.h>
#include <vector>

class Actor;
struct IPropertyWidget;
struct Properties;
struct Property;
class QWidget;
class QGridLayout;
class QScrollArea;

//@Todo: figure out if doing a ComponentTreeWidget to be able to sort through an Actor's
//components and see their spatial relationships (like Unreal) would be good. 
//Previously attempted it and didn't work out well with the mess of code in PropertiesDock.

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock();
	void DisplayActorProperties(Actor* actor);
	void IterateOverProperties(Properties& props, int& currentGridRow);
	void ResetPropertyWidgetValues();
	void Clear();

private:
	template <typename PropType, typename WidgetType>
	void CreateWidget(Property& prop, int row) 
	{
	    auto widget = new WidgetType(prop);
		actorPropsGridLayout->addWidget(widget, row, propertyDataColumn);
	    propertyWidgetsToUpdate.emplace_back(static_cast<IPropertyWidget*>(widget));
	}

	std::vector<IPropertyWidget*> propertyWidgetsToUpdate;

	QWidget* actorPropsWidget = nullptr;
	QGridLayout* actorPropsGridLayout = nullptr;
	QScrollArea* actorPropsScrollArea = nullptr;

	Actor* previousActor = nullptr;
	Actor* currentDisplayingActor = nullptr;

	static const int propertyNameColumn = 0;
	static const int propertyDataColumn = 1;
};
