export module Core.PropertyTypes;

import <string>;

import Core.VEnum;

export namespace PropertyTypes
{
	//Make sure this is only called once at engine startup.
	void SetupPropertyTypesVEnum();

	VEnum GetPropertyTypes();

	inline static std::string Bool = "Bool";
	inline static std::string Int = "Int";
	inline static std::string Float = "Float";
	inline static std::string String = "String";
};

//Todo: wrapped in std::atomic
VEnum propertyTypesEnum;

void PropertyTypes::SetupPropertyTypesVEnum()
{
	propertyTypesEnum.Reset();

	propertyTypesEnum.Add(PropertyTypes::Bool);
	propertyTypesEnum.Add(PropertyTypes::Int);
	propertyTypesEnum.Add(PropertyTypes::Float);
	propertyTypesEnum.Add(PropertyTypes::String);
}

VEnum PropertyTypes::GetPropertyTypes()
{
	return propertyTypesEnum;
}
