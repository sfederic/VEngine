#pragma once
#include <vector>

struct ICommand;

//Currently used for undo/redo commands in the editor, but is generic enough with ICommand
//that it could be used during gameplay and for other systems.
struct CommandSystem
{
private:
	uint32_t commandIndex = 0;

public:
	std::vector<ICommand*> commands;

	void Tick();
	void Add(ICommand* command);
	void Reset();

	//Wind back or forward to a selected command
	void WindToCommand(uint32_t windCommandIndex);

private:
	void Undo();
	void Redo();
};

extern CommandSystem commandSystem;
