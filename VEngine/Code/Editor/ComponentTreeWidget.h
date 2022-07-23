#pragma once
#include <qtreewidget.h>

struct Component;

class ComponentTreeWidget : public QTreeWidget
{
private:

public:
	ComponentTreeWidget(QWidget* parent = nullptr);

	void AddComponentAsItem(Component* component);
	void Clear();
};
