export module Core.VAssert;

import <string>;
import <stdexcept>;

// Custom assert function that throws an exception
export void VAssert(bool condition, const std::string& message)
{
	if (!condition)
	{
		throw std::runtime_error(message);
	}
}
