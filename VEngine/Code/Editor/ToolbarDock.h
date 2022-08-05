#pragma once
#include <qdockwidget.h>

class QPushButton;
class QComboBox;
class QLabel;
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

	QLabel* worldLocalTransformSetting = nullptr;

	//values for pick mode combobox
	QString pickerStringActor = "Actor";
	QString pickerStringComponent = "Component";

	ToolbarDock();
	void SetTransformDisplay(std::string currentTransformSettingName);

private:
	void StartPlay();
	void ResetWorldState();
	void SaveWorld();
	void SetTranslationSnapValue();
	void SetScaleSnapValue();
	void SetRotationSnapValue();
	void PickModeChanged(const QString& item);
};
