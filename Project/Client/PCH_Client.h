
#ifndef PCH_CLIENT
#define PCH_CLIENT

#include <UserContents/PCH_UserContents.h>

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, "UserContents/x64/Debug/UserContents.lib")
#else _DEBUG
#pragma comment(lib, "UserContents/x64/Release/UserContents.lib")
#endif _DEBUG

#else _WIN64
#error "64비트만 지원"
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "Resource.h"


#endif
