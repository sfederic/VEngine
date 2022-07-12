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
		value->filename.assign(
			VString::GetSubStringAtFoundOffset(filename.toStdString(), AssetBaseFolders::shader));
		setToolTip(filepath);

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
