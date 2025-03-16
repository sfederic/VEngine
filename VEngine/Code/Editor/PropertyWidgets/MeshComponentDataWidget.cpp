
#include "MeshComponentDataWidget.h"
#include <qfiledialog.h>
import Core.VString;
import Render.RenderPropertyStructs;
#include "Asset/AssetBaseFolders.h"

MeshComponentDataWidget::MeshComponentDataWidget(Property& prop_)
{
	prop = prop_;
	value = prop.GetData<MeshComponentData>();

	setText(QString::fromStdString(value->filename));
	connect(this, &QPushButton::clicked, this, &MeshComponentDataWidget::SetValue);
}

void MeshComponentDataWidget::SetValue()
{
	QString filepath = QFileDialog::getOpenFileName(this,
		tr("Set Mesh"),
		QString::fromStdString(AssetBaseFolders::mesh),
		tr("Meshes (*.vmesh)"),
		nullptr,
		QFileDialog::Option::DontUseNativeDialog);

	QFileInfo info(filepath);
	QString filename = info.fileName();

	if (!filename.isEmpty())
	{
		IPropertyWidget::SetValue<MeshComponentData>(prop);

		value->filename.assign(
			VString::GetSubStringAtFoundOffset(filename.toStdString(), AssetBaseFolders::mesh));

		setToolTip(filepath);

		prop.change(prop);
		ResetValue();
	}
}

void MeshComponentDataWidget::ResetValue()
{
	if (value)
	{
		setText(QString::fromStdString(value->filename));
	}
}
