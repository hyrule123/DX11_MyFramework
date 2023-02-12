#pragma once

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <queue>
#include <iterator>



using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::make_pair;
using std::wstring;
using std::string;
using std::queue;

#include <typeinfo>
#include <typeindex>
#include <assert.h>

#include <wrl.h>
using namespace Microsoft::WRL;

#include <d3d11.h>					// DirectX11
#include <d3dcompiler.h>			// Shader ÄÄÆÄÀÏ
#include <DirectXMath.h>			// DX Math
#include <DirectXPackedVector.h>	

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;



#include "define.h"
#include "struct.h"
#include "func.h"
#include "Shader_Header_Register.hlsli"

#include "CSingleton.h"


