#pragma once
#include "IPropertyWidget.h"
#include <qwidget.h>
#include <vector>

struct Actor;
class QListWidget;
class QPushButton;
class QListWidgetItem;

struct ActorListWidget : public QWidget, IPropertyWidget
{
	QListWidget* actorNameListWidget;
	QPushButton* addButton;
	QPushButton* deleteButton;

	std::vector<Actor*>* value;

	ActorListWidget(Property value_);
	void SetValue(QListWidgetItem* item);
	virtual void ResetValue() override;

	void AddItem();
	void DeleteItems();
};
