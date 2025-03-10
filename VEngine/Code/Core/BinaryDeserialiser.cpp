import Core.BinaryDeserialiser;
import Core.Properties;
import std.core;
import Render.RenderPropertyStructs;

void BinaryDeserialiser::Deserialise(Properties& props)
{
	for (auto& [name, prop] : props.propMap)
	{
		if (props.CheckType<std::string>(name))
		{
			auto str = props.GetData<std::string>(name);
			ReadString(str);
		}
		else if (props.CheckType<std::wstring>(name))
		{
			auto str = props.GetData<std::wstring>(name);
			ReadWString(str);
		}
		else if (props.CheckType<MeshComponentData>(name))
		{
			auto meshComponentData = props.GetData<MeshComponentData>(name);
			ReadString(&meshComponentData->filename);
		}
		else if (props.CheckType<TextureData>(name))
		{
			auto textureData = props.GetData<TextureData>(name);
			ReadString(&textureData->filename);
		}
		else if (props.CheckType<VEnum>(name))
		{
			auto vEnum = props.GetData<VEnum>(name);
			std::string readStr;
			ReadString(&readStr);
			vEnum->SetValue(readStr);
		}
		else
		{
			fread(prop.data, prop.size, 1, file);
		}
	}
}

void BinaryDeserialiser::ReadString(std::string* str)
{
	size_t stringSize = 0;
	fread(&stringSize, sizeof(size_t), 1, file);

	std::vector<char> buff(stringSize);
	fread(buff.data(), stringSize, 1, file);
	assert(buff.size() == stringSize);

	const std::string newStr(buff.data(), stringSize);
	*str = newStr;
}

void BinaryDeserialiser::ReadWString(std::wstring* wstr)
{
	std::string str;
	ReadString(&str);
	*wstr = VString::stows(str);
}
