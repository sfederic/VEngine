#include "ShaderDataWidget.h"
#include <qfiledialog.h>
#include "Render/RenderTypes.h"

ShaderDataWidget::ShaderDataWidget(Property prop_)
{
	prop = prop_;
	value = (ShaderData*)prop.data;

	setText(QString::fromStdString(value->filename));
	connect(this, &QPushButton::clicked, this, &ShaderDataWidget::SetValue);
}

void ShaderDataWidget::SetValue()
{
	QString filepath = QFileDialog::getOpenFileName(this,
		tr("Set Shader"), "Code/Render/Shaders/", tr("Shader Files (*.hlsl)"));

	QFileInfo info(filepath);
	QString filename = info.fileName();

	if (!filename.isEmpty())
	{
		value->filename = filename.toStdString();
		prop.change(value);
		ResetValue();
	}
}

void ShaderDataWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdString(value->filename));
	}
}
