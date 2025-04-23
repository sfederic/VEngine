export module Editor.PropertyWidgets.IPropertyWidget;

import Commands.CommandSystem;
import Core.Property;

export class IPropertyWidget
{
public:
	Property prop;
	virtual void ResetValue() = 0;

	template <typename T>
	void SetValue(Property& prop)
	{
		CommandSystem::Get().AddCommand<T>(prop);
	}
};
