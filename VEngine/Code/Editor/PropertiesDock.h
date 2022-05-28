#pragma once
#include <qdockwidget.h>
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

private:
	std::unordered_map<std::type_index, std::function<void(Property&, int)>> typeToFunctionMap;

	void CreateBoolWidget(Property& prop, int currentGridRow);
	void CreateIntWidget(Property& prop, int currentGridRow);
	void CreateFloatWidget(Property& prop, int currentGridRow);
	void CreateFloat3Widget(Property& prop, int currentGridRow);
	void CreateFloat4Widget(Property& prop, int currentGridRow);
	void CreateFloat2Widget(Property& prop, int currentGridRow);
	void CreateVectorWidget(Property& prop, int currentGridRow);
	void CreateStringWidget(Property& prop, int currentGridRow);
	void CreateWStringWidget(Property& prop, int currentGridRow);
	void CreateTextureDataWidget(Property& prop, int currentGridRow);
	void CreateShaderDataWidget(Property& prop, int currentGridRow);
	void CreateMeshComponentDataWidget(Property& prop, int currentGridRow);
	void CreateTransformWidget(Property& prop, int currentGridRow);
	void CreateVEnumWidget(Property& prop, int currentGridRow);
};
