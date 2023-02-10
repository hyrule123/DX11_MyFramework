#pragma once

#ifdef _DEBUG
#pragma comment (lib, "UtilLib/UtilLib_Debug")

#else
#pragma comment (lib, "UtilLib/UtilLib")

#endif
#include <UtilLib/json.h>