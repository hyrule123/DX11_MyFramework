#pragma once

#include "framework.h"

// Engine Library
#include <Engine\global.h>
#include <Engine\CEngine.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine//Engine_d")
#else
#pragma comment(lib, "Engine//Engine")
#endif
