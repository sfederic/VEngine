#pragma once
#include <qdockwidget.h>
#include <qgridlayout.h>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <functional>

struct Actor;
struct IPropertyWidget;
struct Properties;
struct Property;
class QWidget;
class QGridLayout;
class QScrollArea;

struct PropertiesDock : public QDockWidget
{
	QWidget* actorPropsWidget = nullptr;
	QGridLayout* actorPropsGridLayout = nullptr;
	QScrollArea* actorPropsScrollArea = nullptr;

	std::vector<IPropertyWidget*> propertyWidgetsToUpdate;

	Actor* previousActor = nullptr;

private:
	static const int propertyNameColumn = 0;
	static const int propertyDataColumn = 1;

public:
	PropertiesDock();
	void DisplayActorProperties(Actor* actor);
	void IterateOverProperties(Properties& props, int& currentGridRow);
	void ResetPropertyWidgetValues();
	void Clear();

	std::unordered_map<std::type_index, std::function<void(Property&, int)>> typeToFunctionMap;

	 template <typename PropType, typename WidgetType>
	 void CreateWidget(Property& prop, int row) 
	 {
	     auto widget = new WidgetType(prop);
		 actorPropsGridLayout->addWidget(widget, row, propertyDataColumn);
	     propertyWidgetsToUpdate.push_back((IPropertyWidget*)widget);
	 }
};
