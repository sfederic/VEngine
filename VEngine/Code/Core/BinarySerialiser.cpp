#include "vpch.h"
#include "BinarySerialiser.h"
#include "Core/VEnum.h"

void BinarySerialiser::Serialise(Properties& props)
{
	for (const auto& [name, prop] : props.propMap)
	{
		if (props.CheckType<std::string>(name))
		{
			auto str = props.GetData<std::string>(name);
			WriteString(*str);
		}
		else if (props.CheckType<std::wstring>(name))
		{
			auto wstr = props.GetData<std::wstring>(name);
			WriteWString(*wstr);
		}
		else if (props.CheckType<MeshComponentData>(name))
		{
			auto meshComponentData = props.GetData<MeshComponentData>(name);
			WriteString(meshComponentData->filename);
		}
		else if (props.CheckType<TextureData>(name))
		{
			auto textureData = props.GetData<TextureData>(name);
			WriteString(textureData->filename);
		}
		else if (props.CheckType<VEnum>(name))
		{
			auto vEnum = props.GetData<VEnum>(name);
			WriteString(vEnum->GetValue());
		}
		else
		{
			fwrite(prop.data, prop.size, 1, file);
		}
	}
}

void BinarySerialiser::WriteString(const std::string str)
{
	const size_t stringSize = str.length();
	fwrite(&stringSize, sizeof(size_t), 1, file);
	fwrite(str.c_str(), stringSize, 1, file);
}

void BinarySerialiser::WriteWString(const std::wstring wstr)
{
	const std::string str = VString::wstos(wstr);
	WriteString(str);
}
