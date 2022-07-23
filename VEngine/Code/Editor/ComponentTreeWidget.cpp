#include "vpch.h"
#include "ComponentTreeWidget.h"
#include "Components/Component.h"

ComponentTreeWidget::ComponentTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    setHeaderLabel("Components");
}

void ComponentTreeWidget::AddComponentAsItem(Component* component)
{
    auto item = new QTreeWidgetItem(this);

    //split string into name and system 
    item->setText(0, QString::fromStdString(component->name));
    item->setFlags(item->flags());
}

void ComponentTreeWidget::Clear()
{
    clear();
}
