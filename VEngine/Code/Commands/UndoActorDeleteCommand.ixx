export module Commands.UndoActorDeleteCommand;

import Commands.ICommand;

class IActorSystem;

class UndoActorDeleteCommand : public ICommand
{
public:
	UndoActorDeleteCommand(IActorSystem* actorSystem) : _actorSystem(actorSystem) {}

	void Execute() override;

private:
	IActorSystem* _actorSystem;
};
