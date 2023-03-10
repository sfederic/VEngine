#pragma once
#include <qdockwidget.h>

class QPushButton;
class QComboBox;
class QLabel;
struct SpinBox;

class ToolbarDock : public QDockWidget
{
public:
	ToolbarDock();
	void SetTransformDisplay(std::string currentTransformSettingName);
	void SetPlayButtonText();

	void StartPlay();
	void ResetWorldState();
	void SaveWorld();
	void SetTranslationSnapValue();
	void SetScaleSnapValue();
	void SetRotationSnapValue();
	void PickModeChanged(const QString& item);
	void LocalWorldTransformValueChanged(const QString& item);

	QPushButton* playButton = nullptr;
	QPushButton* resetWorldButton = nullptr;;
	QPushButton* saveWorldButton = nullptr;;

	SpinBox* translationSnapSpinBox = nullptr;;
	SpinBox* scaleSnapSpinBox = nullptr;;
	SpinBox* rotationSnapSpinBox = nullptr;;

	SpinBox* cameraMoveSpeedSpinBox = nullptr;;

	QComboBox* worldEditorPickMode = nullptr;
	QComboBox* worldLocalTransformSetting = nullptr;

	//values for pick mode combobox
	QString pickerStringActor = "Actor";
	QString pickerStringComponent = "Component";
};
