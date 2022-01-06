#pragma once
#include "WidgetComponent.h"
#include "DialogueStructures.h"

struct DialogueWidget;

//Looks like about 70 characters per line is the limit.
struct DialogueComponent : WidgetComponent
{
	COMPONENT_SYSTEM(DialogueComponent)

private:
	int currentLine = 0;
	int spawnDialogueCurrentLine = 0;

public:
	Dialogue dialogue;
	DialogueWidget* dialogueWidget = nullptr;

	//Make sure that for conversations, spawnDialogue is only set on one NPC. Else dialogue will clash.
	Dialogue spawnDialogue;

	//previous active widget in conversation to hide when text is progressed.
	DialogueWidget* previousActiveDialogueWidget = nullptr;

	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	void ShowSpawnDialogue();
	void NextSpawnLine();

	//Returns false if dialogue is at end
	bool NextLine();
	bool ShowTextAtActor();

	DialogueData* GetCurrentLine();
};
