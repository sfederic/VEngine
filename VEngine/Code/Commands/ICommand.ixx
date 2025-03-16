export module Commands.ICommand;

import <string>;

export class ICommand
{
public:
	virtual ~ICommand() = default;
	virtual void Execute() = 0;
};
