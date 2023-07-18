#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <string_view>
#include <queue>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <random>
#include <type_traits>
#include <thread>
#include <future>
#include <memory>
#include <fstream>
#include <span>

using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::make_pair;
using std::shared_ptr;

using std::wstring;
using std::string;
using std::u8string;
using std::string_view;
using std::wstring_view;
using std::u8string_view;

using std::queue;


#include <typeinfo>
#include <typeindex>
#include <assert.h>

#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h>					// DirectX11
#include <d3dcompiler.h>			// Shader 컴파일
#include <DirectXMath.h>			// DX Math
#include <DirectXPackedVector.h>	

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <DirectXTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex/DirectXTex_Debug")
#else 
#pragma comment(lib, "DirectXTex/DirectXTex_Release")
#endif

#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

