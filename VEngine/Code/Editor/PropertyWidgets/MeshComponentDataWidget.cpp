#include "vpch.h"
#include "MeshComponentDataWidget.h"
#include <qfiledialog.h>
#include "Core/VString.h"
#include "Render/RenderPropertyStructs.h"
#include "Asset/AssetPaths.h"

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
		tr("Meshes (*.vmesh)"));

	QFileInfo info(filepath);
	QString filename = info.fileName();

	if (!filename.isEmpty())
	{
		value->filename.assign(
			VString::GetSubStringAtFoundOffset(filename.toStdString(), AssetBaseFolders::mesh));

		setToolTip(filepath);

		prop.change(value);
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
