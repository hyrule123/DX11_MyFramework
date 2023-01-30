#pragma once

//
//
//
//값이 바뀔 경우 HLSL 쪽에서도 변경시켜주어야 하는 값들
//
//
//


enum eCONST_BUFFER_TYPE : UINT
{
	eCONST_BUFFER_TRANSFORM,	// b0
	eCONST_BUFFER_MATERIAL,	// b1
	eCONST_BUFFER_GLOBAL, //b2
	eCONST_BUFFER_SBUFFERINFO, //b3

	eCONST_BUFFER_END,
};

//상수 버퍼 'SBUFFERINFO' 내부의 인덱스 번호를 지정하는 열거체
enum eSBUFFER_SHARED_CBUFFER_IDX : UINT
{
	LIGHT2D,
	TILE,
	END
};

enum eSCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,

};
#define COLOR_KEY VEC4_3


enum eTEX_PARAM
{
	eTEX_0,
	eTEX_1,
	eTEX_2,
	eTEX_3,
	eTEX_4,
	eTEX_5,
	eTEX_6,
	eTEX_7,

	eTEX_END,
};

enum class eLIGHT_TYPE : UINT
{
	eLIGHT_DIRECTIONAL,	//직사광선
	eLIGHT_POINT,		//점광원
	eLIGHT_SPOTLIGHT	//스포트라이트
};






//=====================================================
//                 STRUCT
//==========================================

//구조화 버퍼의 공유 자원을 이 상수버퍼에 모아서 전달
struct tSBufferInfo
{
	UINT32 uSBufferCount;
	Vec3 Padding;
};
extern tSBufferInfo g_arrStructBufferInfo[(UINT)eSBUFFER_SHARED_CBUFFER_IDX::END];


struct tMtrlConst
{
	int arrInt[4];
	float arrFloat[4];
	Vec2 arrV2[4];
	Vec4 arrV4[4];
	Matrix arrMat[4];
	int arrbTex[eTEX_PARAM::eTEX_END];
};

struct tTransform
{
	Matrix matWorld;
	Matrix matWVP;
};

extern tTransform g_transform;
extern Matrix g_matViewProj;

struct tLightColor
{
	Vec4 vDiffuse;
	Vec4 vAmbient;
};

struct tLightInfo
{
	tLightColor LightColor;

	Vec4 vLightWorldPos;

	Vec4 vLightDir;	//직사광선 또는 스포트라이트의 방향

	float fRadius;	//점광원 또는 스포트라이트의 거리
	float fAngle;	//스포트라이트의 부채꼴 각도
	UINT LightType;
	int padding;
};

// TileMap
struct tTile
{
	Vec2 vLeftTop;
	Vec2 vSlice;
};

//RenderMgr에서 보낼 구조체 변수
struct tGlobalValue
{
	Vec2  Resolution;
	float DeltaTime;
	float AccTime;	//프로그래밍 작동 시간
};
extern tGlobalValue g_GlobalVal;

struct tDebugShapeInfo
{
	eSHAPE_TYPE	eShape;
	float		fLifeSpan;
	Vec2		bytepadding;
	Matrix		matWorld;
	Vec4		vColor;
};