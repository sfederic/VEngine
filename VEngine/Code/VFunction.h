#pragma once
#include <functional>
#include <vector>

//Acts as a defined information for a function, holding types via packed params.
template <typename... Args>
class VFunction : public IFunction
{
private:
	std::string name;
	std::function<void(Args...)> function;

public:
	VFunction(std::string name_, std::function<void(Args...)> function_)
		: name(name_), function(function_)
	{
		IterateTypes();
		functionSystem.AddFunction(name, this);
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
