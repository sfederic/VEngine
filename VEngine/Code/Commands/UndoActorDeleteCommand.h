#pragma once

#include "ICommand.h"

class IActorSystem;

class UndoActorDeleteCommand : public ICommand
{
public:
	UndoActorDeleteCommand(IActorSystem* actorSystem) : _actorSystem(actorSystem) {}

	void Execute() override;

private:
	IActorSystem* _actorSystem;
};
