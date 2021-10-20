#pragma once
#include "Component.h"
#include "ComponentSystem.h"
#include "DialogueStructures.h"

struct WidgetComponent;

struct DialogueComponent : Component
{
	COMPONENT_SYSTEM(DialogueComponent)

private:
	int currentLine = 0;

public:
	Dialogue dialogue;
	WidgetComponent* previousWidgetComponent = nullptr;

	virtual void Tick(float deltaTime) override;
	virtual void Start() override;
	virtual void Create() override;
	virtual Properties GetProps() override;

	//Returns false if dialogue is at end
	bool NextLine();
	void ShowTextAtActor();
	DialogueData* GetCurrentLine();
};
