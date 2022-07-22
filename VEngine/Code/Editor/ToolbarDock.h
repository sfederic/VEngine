#pragma once
#include <qdockwidget.h>

class QPushButton;
class QComboBox;
struct SpinBox;

struct ToolbarDock : public QDockWidget
{
	QPushButton* playButton;
	QPushButton* resetWorldButton;
	QPushButton* saveWorldButton;

	SpinBox* translationSnapSpinBox;
	SpinBox* scaleSnapSpinBox;
	SpinBox* rotationSnapSpinBox;

	SpinBox* cameraMoveSpeedSpinBox;

	QComboBox* worldEditorPickMode = nullptr;

	//values for pick mode combobox
	QString pickerStringActor = "Actor";
	QString pickerStringComponent = "Component";

	ToolbarDock();

private:
	void StartPlay();
	void ResetWorldState();
	void SaveWorld();
	void SetTranslationSnapValue();
	void SetScaleSnapValue();
	void SetRotationSnapValue();
};
