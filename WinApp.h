#pragma once
#include <string>
#include <Windows.h>
#include <cstdint>
#include<format>

void Log(const std::string& message);

// クライアント領域のサイズ
const int32_t kClientWidth = 1280;
const int32_t kClientHeight = 720;
