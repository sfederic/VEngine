#pragma once
#include "IEditor.h"
#include <qobject.h>

struct QtEditor : IEditor, QObject
{
	struct QApplication* app;
	struct EditorMainWindow* mainWindow;

	QString editorTitle = "VEngine | Vagrant Tactics | ";

	void Init(int argc, char* argv[]) override;
	void Tick() override;
	void SetMousePos() override;
	virtual void Log(const std::wstring logMessage) override;
	virtual void Log(const std::string logMessage) override;
	virtual void SetActorProps(Actor* actor) override;
	virtual void UpdateWorldList() override;
	virtual void AddActorToWorldList(Actor* actor) override;
	virtual void RemoveActorFromWorldList(Actor* actor) override;
	virtual void RefreshAssetList() override;
	virtual void ClearProperties() override;
	virtual void SelectActorInWorldList() override;
	virtual void SetPlayButtonText(std::string text) override;
	virtual void SetEditorTitle(const std::string title) override;
	void SetEditorFont();
	void EnableDarkMode();
};
