#pragma once

#include "Resource.h"
#include "../ScreenRenderer/ScreenRenderer.h"
#ifdef _WIN64
#error Only supports Win32, QQ:515311445
#else
#ifdef _DEBUG
#error Only supports Release, QQ:515311445
#else
#pragma comment(lib,"../ScreenRenderer/Release/ScreenRenderer.lib")
#endif
#endif

ScreenRenderer screenRenderer;
BOOL OpenFile();
BOOL loadMedia(TCHAR *szFileName);
BOOL SaveFile();
void setMode(DispMode mode);