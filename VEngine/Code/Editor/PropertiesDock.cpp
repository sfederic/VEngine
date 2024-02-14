#include "vpch.h"
#include "PropertiesDock.h"
#include <qwidget.h>
#include <qlabel.h>
#include <qboxlayout.h>
#include <qscrollarea.h>
#include "Actors/Actor.h"
#include "PropertyWidgets/BoolWidget.h" 
#include "PropertyWidgets/Float2Widget.h"
#include "PropertyWidgets/Float3Widget.h"
#include "PropertyWidgets/Float4Widget.h"
#include "PropertyWidgets/FloatWidget.h"
#include "PropertyWidgets/IntWidget.h"
#include "PropertyWidgets/VectorWidget.h"
#include "PropertyWidgets/StringWidget.h"
#include "PropertyWidgets/WStringWidget.h"
#include "PropertyWidgets/TextureDataWidget.h"
#include "PropertyWidgets/MeshComponentDataWidget.h"
#include "PropertyWidgets/VEnumWidget.h"
#include "Core/VEnum.h"
#include "Render/RenderPropertyStructs.h"

std::unordered_map<std::type_index, std::function<void(Property&, int)>> typeToFunctionMap;

PropertiesDock::PropertiesDock() : QDockWidget("Properties")
{
	setFixedWidth(400);

	actorPropsScrollArea = new QScrollArea(this);

	actorPropsWidget = new QWidget(actorPropsScrollArea);
	actorPropsWidget->setFixedWidth(400);

	actorPropsGridLayout = new QGridLayout(actorPropsWidget);

	actorPropsWidget->setLayout(actorPropsGridLayout);

	actorPropsScrollArea->setWidget(actorPropsWidget);

	//Setup filter bar
	propertyFilterLineEdit = new QLineEdit(this);
	propertyFilterLineEdit->setPlaceholderText("Filter props...");
	connect(propertyFilterLineEdit, &QLineEdit::textChanged, this, &PropertiesDock::FilterProperties);

	auto vBoxLayout = new QVBoxLayout(this);
	vBoxLayout->addWidget(propertyFilterLineEdit);
	vBoxLayout->addWidget(actorPropsScrollArea);

	setLayout(vBoxLayout);

	//Setup map
	typeToFunctionMap[typeid(bool)] = [&](Property& prop, int row) { CreateWidget<bool, BoolWidget>(prop, row); };
	typeToFunctionMap[typeid(int)] = [&](Property& prop, int row) { CreateWidget<int, IntWidget>(prop, row); };
	typeToFunctionMap[typeid(float)] = [&](Property& prop, int row) { CreateWidget<float, FloatWidget>(prop, row); };
	typeToFunctionMap[typeid(XMFLOAT3)] = [&](Property& prop, int row) { CreateWidget<XMFLOAT3, Float3Widget>(prop, row); };
	typeToFunctionMap[typeid(XMFLOAT4)] = [&](Property& prop, int row) { CreateWidget<XMFLOAT4, Float4Widget>(prop, row); };
	typeToFunctionMap[typeid(XMFLOAT2)] = [&](Property& prop, int row) { CreateWidget<XMFLOAT2, Float2Widget>(prop, row); };
	typeToFunctionMap[typeid(XMVECTOR)] = [&](Property& prop, int row) { CreateWidget<XMVECTOR, VectorWidget>(prop, row); };
	typeToFunctionMap[typeid(std::string)] = [&](Property& prop, int row) { CreateWidget<std::string, StringWidget>(prop, row); };
	typeToFunctionMap[typeid(std::wstring)] = [&](Property& prop, int row) { CreateWidget<std::wstring, WStringWidget>(prop, row); };
	typeToFunctionMap[typeid(TextureData)] = [&](Property& prop, int row) { CreateWidget<TextureData, TextureDataWidget>(prop, row); };
	typeToFunctionMap[typeid(MeshComponentData)] = [&](Property& prop, int row) { CreateWidget<MeshComponentData, MeshComponentDataWidget>(prop, row); };
	typeToFunctionMap[typeid(VEnum)] = [&](Property& prop, int row) { CreateWidget<VEnum, VEnumWidget>(prop, row); };
}

void PropertiesDock::DisplayActorProperties(Actor* actor)
{
	//Set the inner values in the Qt widgets instead of remaking all the widgets below
	if (previousActor == actor)
	{
		ResetPropertyWidgetValues();
		return;
	}

	currentDisplayingActor = actor;

	if (actorPropsWidget && actorPropsGridLayout)
	{
		delete actorPropsWidget;
		actorPropsWidget = nullptr;
	}

	propertyWidgetsToUpdate.clear();

	actorPropsWidget = new QWidget(actorPropsScrollArea);

	actorPropsGridLayout = new QGridLayout(actorPropsWidget);
	actorPropsGridLayout->setAlignment(Qt::AlignTop);

	//Iterate over actor properties
	int gridRow = 0;

	actorPropsGridLayout->addWidget(
		new QLabel(actor->GetName().c_str(), actorPropsWidget), gridRow, 0, 1, 1);

	auto actorProps = actor->GetProps();
	IterateOverProperties(actorProps, gridRow);

	//Iterate over actor's component properties
	for (auto component : actor->GetAllComponents())
	{
		actorPropsGridLayout->addWidget(
			new QLabel(component->name.c_str(), actorPropsWidget), gridRow, 0, 1, 1);

		auto componentProps = component->GetProps();
		IterateOverProperties(componentProps, gridRow);
	}

	actorPropsWidget->setLayout(actorPropsGridLayout);
	actorPropsWidget->setFixedWidth(400);

	actorPropsScrollArea->setWidget(actorPropsWidget);

	setWidget(actorPropsScrollArea);

	FilterProperties();

	previousActor = actor;
}

void PropertiesDock::IterateOverProperties(Properties& props, int& currentGridRow)
{
	//Set title to separate components/actor properties
	auto titleLabel = new QLabel(props.title.c_str(), this);
	actorPropsGridLayout->addWidget(titleLabel, currentGridRow, 1, 1, 1);
	currentGridRow++;

	//Properties
	for (auto& [name, prop] : props.propMap)
	{
		if (prop.hide)
		{
			continue;
		}

		//Set property name onto label
		actorPropsGridLayout->addWidget(new QLabel(name.c_str()), currentGridRow, propertyNameColumn, Qt::AlignRight);

		//Call create widget function based on property type
		auto& createWidgetFunc = typeToFunctionMap.find(prop.info.value())->second;
		createWidgetFunc(prop, currentGridRow);

		currentGridRow++;
	}
}

void PropertiesDock::Clear()
{
	previousActor = nullptr;
	propertyWidgetsToUpdate.clear();

	if (actorPropsGridLayout && actorPropsWidget)
	{
		delete actorPropsGridLayout;
		actorPropsGridLayout = nullptr;

		delete actorPropsWidget;
		actorPropsWidget = nullptr;
	}
}

void PropertiesDock::FilterProperties()
{
	if (actorPropsGridLayout == nullptr) { return; }

	const QString propertyFilterText = propertyFilterLineEdit->text().toLower();

	for (int i = 0; i < actorPropsGridLayout->rowCount(); i++)
	{
		auto propertyLabel = static_cast<QLabel*>(actorPropsGridLayout->itemAtPosition(
			i, propertyNameColumn)->widget());
		auto propertyDataWidget = static_cast<QWidget*>(actorPropsGridLayout->itemAtPosition(
			i, propertyDataColumn)->widget());

		if (!propertyLabel->text().toLower().contains(propertyFilterText))
		{
			propertyDataWidget->hide();
			propertyLabel->hide();
		}
		else
		{
			propertyDataWidget->show();
			propertyLabel->show();
		}
	}
}

void PropertiesDock::ResetPropertyWidgetValues()
{
	for (IPropertyWidget* propertyWidget : propertyWidgetsToUpdate)
	{
		propertyWidget->ResetValue();
	}
}
