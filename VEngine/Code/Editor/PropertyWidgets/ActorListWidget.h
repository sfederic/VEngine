#pragma once
#include "IPropertyWidget.h"
#include <qwidget.h>
#include <vector>

struct Actor;
class QListWidget;
class QPushButton;

//TODO: can probably make this sort of widget a template so it can work with all types.
struct ActorListWidget : public QWidget, IPropertyWidget
{
	QListWidget* actorNamelistWidget;
	QPushButton* addButton;
	QPushButton* deleteButton;

	std::vector<Actor*>* value;

	ActorListWidget(Property value_);
	void SetValue();
	virtual void ResetValue() override;

	void AddItem();
	void DeleteItems();
};
