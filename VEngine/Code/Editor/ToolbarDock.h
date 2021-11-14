#pragma once
#include <qdockwidget.h>

class QPushButton;
struct SpinBox;

struct ToolbarDock : public QDockWidget
{
	QPushButton* playButton;
	QPushButton* resetWorldButton;
	QPushButton* saveWorldButton;

	SpinBox* translationSnapSpinBox;
	SpinBox* scaleSnapSpinBox;
	SpinBox* rotationSnapSpinBox;

	ToolbarDock();

private:
	void StartPlay();
	void ResetWorldState();
	void SaveWorld();
	void SetTranslationSnapValue();
	void SetScaleSnapValue();
	void SetRotationSnapValue();
};
