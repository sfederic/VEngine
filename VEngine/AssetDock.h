#pragma once

#include <qdockwidget.h>

class AssetDock : public QDockWidget
{
public:
	AssetDock(const char* title);

	void AssetItemClicked(class QListWidgetItem* listWidgetItem);
};

