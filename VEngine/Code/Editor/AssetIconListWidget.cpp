#include "vpch.h"
#include "AssetIconListWidget.h"

AssetIconListWidget::AssetIconListWidget()
{
	setIconSize(QSize(75, 75));
	setViewMode(QListView::ViewMode::IconMode);
}
