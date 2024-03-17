#pragma once

#include <qlistwidget.h>

//This class exists to filter out any arrow key presses and keyboard searches with an empty key press event.
class AssetIconListWidget : public QListWidget
{
public:
	AssetIconListWidget();
	void keyPressEvent(QKeyEvent* event) override {}
};
