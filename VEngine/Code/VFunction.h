#pragma once
#include "IFunction.h"
#include <functional>

//Acts as a defined information for a function, holding types via packed params.
template <typename... Args>
class VFunction : public IFunction
{
private:
	std::function<void(Args...)> function;

public:
	VFunction(std::string name_, std::function<void(Args...)> function_, std::vector<std::string> argNames_)
		: IFunction(name_)
	{
		argNames = argNames_;
		function = function_;
		IterateTypes();
	}

	void Call(Args... arg)
	{
		function(arg...);
	}

private:
	void IterateTypes()
	{
		argTypes.insert(argTypes.end(), { typeid(Args)... });
	}
};
