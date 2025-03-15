export module Core.Log;

import Editor.Editor;
import Editor.IEditor;
import <cstdarg>;
import <fstream>;
import <chrono>;
import <iostream>;
import <iomanip>;
import <ctime>;

export void Log(std::string logMessage, ...)
{
	va_list args;
	va_start(args, logMessage.c_str());

	char msg[1024];
	_vsnprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	Editor::Get().Log(msg);

	std::ofstream os;
	os.open("Log.txt", std::ios_base::app);

	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	os << std::put_time(std::localtime(&now_c), "%F %T") << " | " << msg << std::endl;
}

export void Log(std::wstring logMessage, ...)
{
	va_list args;
	va_start(args, logMessage.c_str());

	wchar_t msg[1024];
	_vsnwprintf_s(msg, 1024, logMessage.c_str(), args);
	va_end(args);

	Editor::Get().Log(msg);

	std::wofstream os;
	os.open("Log.txt", std::ios_base::app);

	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	os << std::put_time(std::localtime(&now_c), "%F %T") << " | " << msg << std::endl;
}
