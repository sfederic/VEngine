#pragma once
#include <qdockwidget.h>

class QPushButton;
struct SpinBox;

struct ToolbarDock : public QDockWidget
{
	QPushButton* playButton;
	QPushButton* resetWorldButton;

	SpinBox* translationSnapSpinBox;
	SpinBox* scaleSnapSpinBox;
	SpinBox* rotationSnapSpinBox;

	ToolbarDock();
	void StartPlay();
	void ResetWorldState();
	void SetTranslationSnapValue();
	void SetScaleSnapValue();
	void SetRotationSnapValue();
};
