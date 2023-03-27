#pragma once
#include "../WidgetComponent.h"
#include "Gameplay/DialogueStructures.h"

struct DialogueWidget;

//Looks like about 70 characters per line is the limit.
struct DialogueComponent : WidgetComponent
{
	COMPONENT_SYSTEM(DialogueComponent)

private:
	int currentLine = 0;

public:
	Dialogue dialogue;
	DialogueWidget* dialogueWidget = nullptr;

	//previous active widget in conversation to hide when text is progressed.
	DialogueWidget* previousActiveDialogueWidget = nullptr;

	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	//Returns false if dialogue is at end
	bool NextLine();
	bool ShowTextAtActor();

	DialogueData* GetCurrentLine();

	//Functions to call in DialogueTriggers
	bool ConversationNextLine();
	void ConversationShowTextAtActor();
};
