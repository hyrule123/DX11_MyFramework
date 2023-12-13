#ifndef PCH_CONTENTS
#define PCH_CONTENTS

#include <Engine/PCH_Engine.h>

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, "Engine/x64/Debug/Engine.lib")
#else _DEBUG
#pragma comment(lib, "Engine/x64/Release/Engine.lib")
#endif _DEBUG

#else _WIN64

#error "64비트만 지원"

#endif _WIN64


#endif
