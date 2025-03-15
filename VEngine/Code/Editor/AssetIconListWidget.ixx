export module Editor.AssetIconListWidget;

#include <qlistwidget.h>

//This class exists to filter out any arrow key presses and keyboard searches with an empty key press event.
export class AssetIconListWidget : public QListWidget
{
public:
	AssetIconListWidget()
	{
		setIconSize(QSize(75, 75));
		setViewMode(QListView::ViewMode::IconMode);
	}

	void keyPressEvent(QKeyEvent*) override {}
};
