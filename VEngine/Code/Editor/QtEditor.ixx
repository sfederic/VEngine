#include <qobject.h>

export module Editor.QtEditor;

import Editor.IEditor;

class QtEditor : public IEditor, public QObject
{
public:
	class QApplication* app = nullptr;
	class EditorMainWindow* mainWindow = nullptr;

	void Init(int argc, char* argv[]) override;
	void Tick() override;
	void SetMousePos() override;
	void SetMousePosFPSGameplay() override;
	virtual void Log(const std::wstring logMessage) override;
	virtual void Log(const std::string logMessage) override;
	virtual void SetActorProps(Actor* actor) override;
	virtual void UpdateWorldList() override;
	virtual void UpdateSystemsList() override;
	virtual void AddActorToWorldList(Actor* actor) override;
	virtual void RemoveActorFromWorldList() override;
	virtual void RefreshAssetList() override;
	virtual void ClearProperties() override;
	void ResetPropertyWidgetValues();
	virtual void SelectActorInWorldList() override;
	virtual void SetPlayButtonText() override;
	virtual void SetEditorTitle(const std::string title) override;
	virtual void SetCurrentTransformMode(const std::string transformMode) override;

private:
	void SetEditorFont();
	void EnableDarkMode();

	QString editorTitle = "VEngine | Vagrant Tactics | ";
};
