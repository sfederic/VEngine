#pragma once

#include "../Actor.h"
#include "../ActorSystem.h"

struct NoteWidget;

//3D notes placed in world by player.
class NoteActor : public Actor
{
public:
	ACTOR_SYSTEM(NoteActor);

	NoteActor();
	virtual Properties GetProps() override;

	void SetNoteText(std::wstring noteText);
	void AddNoteWidgetToViewport();

private:
	NoteWidget* noteWidget = nullptr;
};
