#pragma once

#include <UtilLib_DLL/json/json.h>

#ifdef _DEBUG
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Debug")
#else
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Release")
#endif


//Json 파일에 부동소수점 변수를 int형태로 변환해서 저장하기 위한 union 구조체들
union Pack_float_int
{
	float f;
	int i;

	Pack_float_int(int _i) : i(_i) {};
	Pack_float_int(float _f) : f(_f) {};
};

union Pack_v2_i64
{
	Vec2 v2;
	INT64 i64;

	Pack_v2_i64(Vec2 _v2) : v2(_v2)
	{}

	Pack_v2_i64(INT64 _i64) : i64(_i64)
	{}
};
