#pragma once

#include "framework.h"

// Engine Library
#include <Engine\global.h>
#include <Engine\CEngine.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_Debug")
#else
#pragma comment(lib, "Engine/Engine")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Script/Script_Debug")
#else
#pragma comment(lib, "Script/Script")
#endif