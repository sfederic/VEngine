#include "vpch.h"
#include "ShaderDataWidget.h"
#include <qfiledialog.h>
#include "Render/RenderTypes.h"
#include "VString.h"
#include "Asset/AssetPaths.h"

ShaderDataWidget::ShaderDataWidget(Property& prop_)
{
	prop = prop_;
	value = prop.GetData<ShaderData>();

	setText(QString::fromStdString(value->shaderItemName));
	connect(this, &QLineEdit::editingFinished, this, &ShaderDataWidget::SetValue);
}

void ShaderDataWidget::SetValue()
{
	value->shaderItemName.assign(text().toStdString());
	prop.change(value);
	ResetValue();
}

void ShaderDataWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdString(value->shaderItemName));
	}
}
