#pragma once
#include "WidgetComponent.h"
#include "DialogueStructures.h"

struct DialogueWidget;

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
	void ShowTextAtActor();
	DialogueData* GetCurrentLine();
};
