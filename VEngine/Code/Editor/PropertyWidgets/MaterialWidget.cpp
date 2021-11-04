#include "MaterialWidget.h"
#include "Render/Material.h"
#include "Render/MaterialSystem.h"

MaterialWidget::MaterialWidget(Material* material_)
{
	material = material_;

	ResetValue();
}

void MaterialWidget::SetValue()
{
	for (auto mat : materialSystem.materials)
	{
		if (mat->name == material->name)
		{

		}
	}
}

void MaterialWidget::ResetValue()
{
	setText(QString::fromStdString(material->name));
}
