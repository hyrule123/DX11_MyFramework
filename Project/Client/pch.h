#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// Engine Library
#include <Engine\global.h>
#include <Engine\cEngine.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_Debug")
#else
#pragma comment(lib, "Engine/Engine_Release")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Script/Script_Debug")
#else
#pragma comment(lib, "Script/Script_Release")
#endif
