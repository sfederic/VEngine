#pragma once
#include <functional>

//A property type that displays a button for an actor/component in the editor to call a function on click.
class ButtonProperty
{
private:
	std::function<void()> clickFunc;
	std::string buttonText;

public:
	ButtonProperty(std::function<void()> clickFunc_, std::string buttonText_) 
		: clickFunc(clickFunc_), buttonText(buttonText_) {}

	auto GetButtonText() { return buttonText; }
	auto GetClickFunction() { return clickFunc; }
};
