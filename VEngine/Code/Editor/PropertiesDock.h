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
class QLineEdit;

class PropertiesDock : public QDockWidget
{
public:
	PropertiesDock();
	void DisplayActorProperties(Actor* actor);
	void IterateOverProperties(Properties& props, int& currentGridRow);
	void ResetPropertyWidgetValues();
	void Clear();

private:
	void FilterProperties();

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
	QLineEdit* propertyFilterLineEdit = nullptr;

	Actor* previousActor = nullptr;
	Actor* currentDisplayingActor = nullptr;

	static const int propertyNameColumn = 0;
	static const int propertyDataColumn = 1;
};
