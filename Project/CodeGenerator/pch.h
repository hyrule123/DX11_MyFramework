#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include <UtilLib_DLL/json/json.h>

#ifdef _DEBUG
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Debug")
#else
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Release")
#endif

using namespace std;

#endif //PCH_H
