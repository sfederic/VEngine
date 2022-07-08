#pragma once
#include <functional>
#include <vector>

//Acts as a defined information for a function, holding types via packed params.
template <typename... Args>
class VFunction
{
private:
	std::string name;
	std::function<void(Args...)> function;
	std::vector<std::type_index> argTypes;

public:
	VFunction(std::string name_, std::function<void(Args...)> function_)
		: name(name_), function(function_)
	{
		IterateTypes();
	}

	void Call(args... arg)
	{
		function(arg...);
	}

	auto GetArgTypes()
	{
		return argTypes;
	}

private:
	void IterateTypes()
	{
		argTypes.insert(argTypes.end(), { typeid(Args)... });
	}
};
