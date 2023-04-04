#pragma once

#ifndef JSON_DLL
#define JSON_DLL
#endif
#include <UtilLib_DLL/json/json.h>

#ifdef _DEBUG
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Debug")
#else
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Release")
#endif



