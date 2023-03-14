#pragma once

#define JSON_KEY(_keyName) constexpr const char* _keyName = #_keyName

#include <UtilLib_DLL/json/json.h>

#ifdef _DEBUG
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Debug")
#else
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Release")
#endif
