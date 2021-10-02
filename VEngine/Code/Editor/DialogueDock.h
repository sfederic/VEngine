#pragma once
#include <qdockwidget.h>

class QTreeWidget;
class QTreeWidgetItem;

//The current dialogue editor is lightly based off of Neverwinter Nights 2's editor.
//REF:https://gdcvault.com/play/1013446/The-Neverwinter-Nights-2-Toolset
//Obsidian's is probably the end goal, their node system is great.
//REF:https://gdcvault.com/play/1025962/Technical-Tools-for-Authoring-Branching

//TODO: look into Qt's json approach if the simple text format isn't enough
//https://doc.qt.io/qt-5/qtcore-serialization-savegame-example.html

//Dock for consversation information
struct DialogueDock : public QDockWidget
{
	QTreeWidget* dialogueTree;

	DialogueDock();

private:
	void PopulateTreeItem(QTreeWidgetItem* item);
	void AddEmptyDialogueLine();
	void AddEmptyBranchDialogueLine();
	void DeleteLine();
	void SaveDialogueToFile();
	void LoadDialogueFile();
};
