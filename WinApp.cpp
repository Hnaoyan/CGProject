#include "WinApp.h"

void Log(const std::string& message) {
	OutputDebugStringA(message.c_str());
}

//std::wstring ConvertString(const std::string& str) {
//	if (str.empty()) {
//		return std::wstring();
//	}
//}
//
