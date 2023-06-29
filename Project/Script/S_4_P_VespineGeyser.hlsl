#include "S_H_SCUnitGround.hlsli"

// ============
// VespineGeyser
// ============
// Topology: TriangleList
// Rasterizer: CULL_BACK(백페이스 컬링)
// Blend State: NONE
// Depth Stencil: Less
// Shader Domain: Opaque
// Instancing: True
// Animation: No

//g_CBuffer_matCam에서 사용
//#define MTRL_SCALAR_MAT_VP                     MTRLDATA_PARAM_SCALAR(MAT, 0)

////CBuffer 또는 SBuffer에서 사용
//#define MTRL_SCALAR_MAT_WVP                     MTRLDATA_PARAM_SCALAR(MAT, 0)
//#define MTRL_SCALAR_MAT_WORLD                   MTRLDATA_PARAM_SCALAR(MAT, 1)

////자신의 VP 행렬을 들고 있는 카메라 상수버퍼에서의 인덱스
//#define MTRL_SCALAR_INT_CAMIDX                  MTRLDATA_PARAM_SCALAR(INT, 0)

////STD2D 기반 쉐이더에서 사용. 플래그 지정
//#define MTRL_SCALAR_STD2D_FLAG                  MTRLDATA_PARAM_SCALAR(INT, 1)
//ENUM_BEGIN(eMTRL_SCALAR_STD2D_FLAG, int)
//    ENUM_MEMBER(USE_VP, int, 1 << 0) //TRUE == WVP 행렬로 전달 FALSE == VP 행렬 따로 전달
//    ENUM_MEMBER(USE_ANIM, int, 1<<1)
//    ENUM_MEMBER(USE_PIVOT, int, 1<<2)
//    ENUM_MEMBER(NEED_FLIP_X, int, 1<<3)
//    ENUM_MEMBER(USE_COLOR_KEY, int, 1<<4)
//ENUM_END

//베스핀 가스는 총 7장의 스프라이트가 하나의 아틀라스 형태로 되어 있음.
//타일셋에 따라 다른 형태를 가지고 있음 -> 각각 타일셋에 맞는 스프라이트를 출력해주어야 한다.
//데이터에 저장된 타일셋의 헥스코드와 동일한 순서대로 저장되어 있는 듯함.
//어차피 스프라이트 번호로 UV를 나눠줘야 하므로 float 형태로 보냄
//#define MTRL_SCALAR_FLOAT_VESPINE_SPRITE		MTRLDATA_PARAM_SCALAR(FLOAT, 0)

//Texture Info
//1. geyser.bmp
//2. 
//3. 
//4. 
//5. 
//6. 
//7. 
//8. 



float4 PS_VespineGeyser(VS_OUT _in) : SV_TARGET
{
	float4 vOutColor = (float4) 0.f;
    
	tMtrlScalarData Data = (tMtrlScalarData) 0.f;
    
	//인스턴싱을 사용하지 않을 경우->상수버퍼의 데이터를 사용
	if (0 == g_CBuffer_SBufferData[eCBUFFER_SBUFFER_SHAREDATA_IDX::MTRL_SCALAR].uSBufferCount)
	{
		Data = g_CBuffer_Mtrl_Scalar;
	}
	
	//인스턴싱을 사용할 경우 -> 구조화 버퍼의 데이터를 사용
	else
	{
		Data = g_SBuffer_Mtrl_Scalar[_in.uInstID];
	}
    
    
    //메인텍스처가 존재하지 않을경우는 무조건 마젠타 색상을 return;
	if (FALSE == g_CBuffer_Mtrl_Tex.bTEX_0)
	{
		vOutColor = float4(1.f, 0.f, 1.f, 1.f);
	}
    
	_in.vUV.x /= 8.f;
	_in.vUV.x += Data.MTRL_SCALAR_FLOAT_VESPINE_SPRITE;

	vOutColor = g_tex_0.Sample(g_Sampler_0, _in.vUV);

	
	//Alpha Check 
	if (0.f == vOutColor.a)
	{
		discard;
	}
    
	//ColorKey Check
	if (eMTRL_SCALAR_STD2D_FLAG::USE_COLOR_KEY & Data.MTRL_SCALAR_STD2D_FLAG)
	{
		if (all(FLT_EPSILON > abs(vOutColor.rgb - Data.MTRL_SCALAR_STD2D_COLORKEY.rgb)))
			discard;
		

		
		//if (all(vOutColor.rgb == Data.MTRL_SCALAR_STD2D_COLORKEY.rgb))
		//	discard;
	}
	
	
	return vOutColor;
	

}
