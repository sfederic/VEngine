import Editor.QtEditor;
import Core.Input;
import Core.Win32Keys;

#include <QApplication>
#include "EditorMainWindow.h"

void QtEditor::Init(int argc, char* argv[])
{
	app = new QApplication(argc, argv); //Init order matters here. Qt wants a QApp before a QMainWindow/QWidget
	mainWindow = new EditorMainWindow();
	windowHwnd = (void*)mainWindow->renderView->winId();

	app->setWindowIcon(QIcon("Icons/engine_icon.png"));

	SetViewportDimensions(mainWindow->renderView->size().width(),
		mainWindow->renderView->size().height());

	SetEditorFont();
	EnableDarkMode();
}

void QtEditor::Tick()
{
	mainWindow->Tick();

	app->processEvents();

	//if (Core::gameplayOn && !UISystem::GetWidgetControlActive())
	//{
	//    SetMousePosFPSGameplay();
	//}
	//else
	{
		SetMousePos();
	}

	//update property dock values if game is running
	if (Core::gameplayOn)
	{
		mainWindow->propertiesDock->ResetPropertyWidgetValues();
	}

	if (Input::GetKeyUp(Keys::F11))
	{
		mainWindow->HideAllDocks();
	}
}

void QtEditor::SetMousePos()
{
	//@Todo: Tried moving Qt from 5.15 to 6.6.2 and this one mapFromGlobal line fucked the whole thing up.
	//It was like an incorrect offset, almost like a pixel scaling issue that just wouldn't work.
	//I tried code to make sure the Qt render viewport and the Renderer d3d11 viewport were equal, also
	//code to make sure pixel dpi was ok across Qt, but those didn't work.
	const QPoint mousePos = mainWindow->renderView->mapFromGlobal(QCursor::pos());
	SetViewportMousePositions(mousePos.x(), mousePos.y());
}

//For first person camera controls during gameplay
void QtEditor::SetMousePosFPSGameplay()
{
	SetViewportDimensions(mainWindow->renderView->size().width(),
		mainWindow->renderView->size().height());

	const QPoint mousePos = mainWindow->renderView->mapFromGlobal(QCursor::pos());
	SetViewportMousePositions(mousePos.x(), mousePos.y());

	const int lCenterOffsetX = (GetViewportWidth() / 2) - GetViewportMouseX();
	const int lCenterOffsetY = (GetViewportWidth() / 2) - GetViewportMouseY();
	SetCenterOffsetPositions(lCenterOffsetX, lCenterOffsetY);

	const QPoint glob = mainWindow->renderView->mapToGlobal(QPoint(GetViewportWidth() / 2, GetViewportHeight() / 2));
	QCursor::setPos(glob);
}

void QtEditor::Log(const std::wstring logMessage)
{
	mainWindow->logDock->Print(logMessage);
}

void QtEditor::Log(const std::string logMessage)
{
	mainWindow->logDock->Print(logMessage);
}

void QtEditor::SetActorProps(Actor* actor)
{
	mainWindow->propertiesDock->DisplayActorProperties(actor);
}

void QtEditor::UpdateWorldList()
{
	mainWindow->worldDock->PopulateWorldActorList();
}

void QtEditor::UpdateSystemsList()
{
	mainWindow->systemDock->PopulateSystemLists();
}

void QtEditor::AddActorToWorldList(Actor* actor)
{
	mainWindow->worldDock->AddActorToList(actor);
}

void QtEditor::RemoveActorFromWorldList()
{
	mainWindow->worldDock->RemoveActorFromList();
}

void QtEditor::RefreshAssetList()
{
	mainWindow->assetDock->AssetFolderClicked();
}

void QtEditor::ClearProperties()
{
	mainWindow->propertiesDock->Clear();
}

void QtEditor::ResetPropertyWidgetValues()
{
	mainWindow->propertiesDock->ResetPropertyWidgetValues();
}

void QtEditor::SetEditorFont()
{
	//Capcom have a cool talk on UI design and fonts for RE Engine
	//REF:https://www.youtube.com/watch?v=YhnIW2XY_wU
	QFont font;
	font.setStyleHint(QFont::SansSerif);
	font.setPixelSize(16);
	app->setFont(font);
}

void QtEditor::EnableDarkMode()
{
	//rgb(77, 77, 77) for nice off grey color
	//rgb(80, 170, 200) - RE Engine UI blue

	//Stole all the scrollbar stuff from https://stackoverflow.com/questions/54595957/how-to-set-a-stylesheet-for-the-qscrollbar-in-a-qscrollarea
	app->setStyleSheet(
		"QWidget { background-color: rgb(45, 45, 45); border-color: rgb(11, 11, 11); border-width: 0.0px; border-style: none; } "

		"QComboBox { border: 2px solid; border-radius: 4px; border-color: rgb(77, 77, 77); height: 30px; } "
		"QComboBox::hover { border-color: rgb(80, 170, 200); } "

		"QDockWidget { background-color: rgb(37, 37, 37); border: 5px solid black; } "

		"QLineEdit { background-color: rgb(40, 40, 40); border: 2px solid; border-radius: 4px; border-color: rgb(77, 77, 77); height: 25px; } "
		"QLineEdit::hover { border-color: rgb(80, 170, 200); } "

		"QSpinBox { background-color: rgb(40, 40, 40); border: 2px solid; border-radius: 4px; border-color: rgb(77, 77, 77); height: 25px; } "
		"QSpinBox::hover { border-color: rgb(80, 170, 200); } "

		"QDoubleSpinBox { background-color: rgb(40, 40, 40); border: 2px solid; border-radius: 4px; border-color: rgb(77, 77, 77); height: 25px; } "
		"QDoubleSpinBox::hover { border-color: rgb(80, 170, 200); } "

		"QPushButton { min-height:30px; min-width:50px; background-color: rgb(53, 53, 53); border-radius: 5px; } "
		"QPushButton:hover { background-color: rgb(80, 80, 80); } "
		"QPushButton:pressed { background-color: rgb(140, 140, 140); } "

		"QHeaderView { color: rgb(210, 210, 210); border: 0px; } "
		"QHeaderView:section { background-color: rgb(37, 37, 37); } "

		"QTabWidget { border: none; } "
		"QTabBar::tab { background: rgb(66, 66, 66); } "

		"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none;  border: 0px; } "
		"QScrollBar:vertical { background-color: #2A2929; width: 15px; margin: 15px 3px 15px 3px; border: 1px transparent #2A2929; border-radius: 4px } "
		"QScrollBar::handle:vertical { background-color: rgb(11, 11, 11); min-height: 5px; border-radius: 4px; } "
		"QScrollBar::sub-line:vertical { margin: 3px 0px 3px 0px; height: 10px; border-image: url(:/qss_icons/rc/up_arrow_disabled.png); width: 10px; subcontrol-position: top; subcontrol-origin: margin; } "
		"QScrollBar::add-line:vertical { margin: 3px 0px 3px 0px; border-image: url(:/qss_icons/rc/down_arrow_disabled.png); height: 10px;width: 10px; subcontrol-position: bottom; subcontrol-origin: margin; } "

		"QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none;  border: 0px; } "
		"QScrollBar:horizontal { height: 15px; margin: 3px 15px 3px 15px; border: 1px transparent #2A2929; border-radius: 4px; } "
		"QScrollBar::handle:horizontal { background-color: rgb(11, 11, 11); min-width: 5px; border-radius: 4px; } "
		"QScrollBar::add-line:horizontal{ margin: 0px 3px 0px 3px; border-image: url(:/qss_icons/rc/right_arrow_disabled.png); width: 10px; height: 10px; subcontrol - position: right; subcontrol - origin: margin; } "
		"QScrollBar::sub-line:horizontal { margin: 0px 3px 0px 3px; border-image: url(:/qss_icons/rc/left_arrow_disabled.png); height: 10px; width: 10px; subcontrol-position: left; subcontrol-origin: margin; } "

		"QTreeWidget::item::selected { color : skyblue } "
	);

	//Set global font to  off white
	QPalette p = app->palette();
	QColor fontColour = QColor::fromRgb(215, 215, 215);
	p.setColor(QPalette::Text, fontColour);
	p.setColor(QPalette::WindowText, fontColour);
	p.setColor(QPalette::ButtonText, fontColour);
	p.setColor(QPalette::BrightText, fontColour);
	app->setPalette(p);
}

void QtEditor::SelectActorInWorldList()
{
	mainWindow->worldDock->SelectActorInList();
}

void QtEditor::SetPlayButtonText()
{
	mainWindow->toolbarDock->SetPlayButtonText();
}

void QtEditor::SetEditorTitle(const std::string title)
{
	QString world = QString::fromStdString(title);
	mainWindow->setWindowTitle(editorTitle + world);
}

void QtEditor::SetCurrentTransformMode(const std::string transformMode)
{
	const int index = mainWindow->toolbarDock->worldLocalTransformSetting->findText(QString::fromStdString(transformMode));
	mainWindow->toolbarDock->worldLocalTransformSetting->setCurrentIndex(index);
}
