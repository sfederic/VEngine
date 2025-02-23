#include "vpch.h"
#include "UndoActorDeleteCommand.h"
#include "Actors/IActorSystem.h"
#include "Actors/Actor.h"
#include "Core/Properties.h"
#include "Core/World.h"
#include "Editor/Editor.h"

void UndoActorDeleteCommand::Execute()
{
	auto lastDeletedActor = _actorSystem->GetLastDeletedActor();
	if (lastDeletedActor == nullptr)
	{
		return;
	}

	//Make sure all components are active when/if previously not before props are copied over
	lastDeletedActor->SetActive(true);

	const auto transform = lastDeletedActor->GetTransform();
	auto undoActor = _actorSystem->SpawnActor(transform);

	auto undoActorProps = undoActor->GetAllProps();
	auto lastDeletedActorProps = lastDeletedActor->GetAllProps();
	Properties::CopyProperties(lastDeletedActorProps, undoActorProps);

	undoActor->Create();
	undoActor->CreateAllComponents();
	undoActor->PostCreate();

	World::AddActorToWorld(undoActor);
	Editor::Get().UpdateWorldList();

	_actorSystem->PopBackLastDeletedActor();
}
