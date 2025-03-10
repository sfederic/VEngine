export module Core.VEnum;

import <string>;
import <vector>;
import <map>;

export class VEnum
{
public:
	std::string GetValue() { return value; }
	void SetValue(const std::string& newValue);
	void Add(const std::string& name);
	std::vector<std::string> GetAllNames();
	bool Contains(std::string _value);
	bool Compare(std::string valueToCompare);
	void Reset();

private:
	int dataIndex = 0;
	std::map<std::string, int> data;
	std::string value;
};
