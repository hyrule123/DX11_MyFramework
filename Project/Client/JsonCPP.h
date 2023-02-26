#pragma once

#define JSON_DLL

#ifdef _DEBUG
#pragma comment (lib, "UtilDLL/UtilDLL_Debug")

#else
#pragma comment (lib, "UtilDLL/UtilDLL")

#endif
#include <UtilDLL/json.h>