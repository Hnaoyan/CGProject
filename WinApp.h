#pragma once
#include <string>
#include <Windows.h>
#include <cstdint>
#include<format>

void Log(const std::string& message);
std::wstring ConvertString(const std::string str);
std::string ConvertString(const std::wstring str);


// クライアント領域のサイズ
const int32_t kClientWidth = 1280;
const int32_t kClientHeight = 720;
