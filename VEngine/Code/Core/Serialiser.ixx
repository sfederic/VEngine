export module Core.Serialiser;

import Core.OpenMode;
import <fstream>;
import <sstream>;
import <unordered_map>;
import <typeindex>;
import <functional>;

struct Property;
struct Properties;

export class Serialiser
{
private:
	std::wofstream ofs;

	//Serialiser uses a stringstream as a temp buffer in memory before writing out to file with std::ofstream.
	//This way if there's a crash while serialising, partial save files won't be written out.
	std::wstringstream ss;

	const std::string filename;
	const OpenMode mode;

	std::unordered_map<std::type_index, std::function<void(Property& prop, std::wstring& name)>> typeToWriteFuncMap;

public:
	Serialiser(const std::string filename_, const OpenMode mode_);
	~Serialiser();
	void Serialise(Properties& props);

	template <typename T>
	void WriteLine(T arg)
	{
		ss << arg << L"\n";
	}
};
