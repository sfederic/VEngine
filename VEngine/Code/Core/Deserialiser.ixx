export module Core.Deserialiser;

import Core.OpenMode;

export struct Properties;
export struct Property;

class Deserialiser
{
public:
	Deserialiser(const std::string& filename, const OpenMode mode);
	~Deserialiser();

	void Deserialise(Properties& props);

	template <typename T>
	void ReadLine(T& arg)
	{
		is >> arg;
	}

	// @Todo: Address potential bug with std::wifstream where type mismatches can lead to infinite loops.
	std::wifstream is; // Input stream for wide characters

private:
	std::unordered_map<std::type_index, std::function<void(Property&)>> typeToReadFuncMap;
};
