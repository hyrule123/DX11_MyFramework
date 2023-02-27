#pragma once

#define JSON_DLL

#ifdef _DEBUG
#pragma comment (lib, "UtilLib_DLL/UtilLib_DLL_Debug")

#else
#pragma comment (lib, "UtilLib_DLL/UtilLib_DLL")

#endif

#include <UtilLib_DLL/json.h>