#pragma once
#include <qdockwidget.h>

class QTreeWidget;

//The current dialogue editor is lightly based off of Neverwinter Nights 2's editor.
//REF:https://gdcvault.com/play/1013446/The-Neverwinter-Nights-2-Toolset
//Obsidian's is probably the end goal, their node system is great.
//REF:https://gdcvault.com/play/1025962/Technical-Tools-for-Authoring-Branching

//Dock for consversation information
struct DialogueDock : public QDockWidget
{
	QTreeWidget* dialogueTree;

	DialogueDock();
	void AddEmptyDialogueLine();
	void AddEmptyBranchDialogueLine();
	void DeleteLine();
	void SaveDialogueToFile();
	void LoadDialogueFile();
};
