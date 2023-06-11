#include "pch.h"
#include "global.h"

//=============================
//전역변수 선언 및 초기화 하는곳
//=============================

tInitSetting g_InitSetting = {};

tCamMatrices g_matCam[(int)eCAMERA_INDEX::END] = {};
tGlobalValue g_GlobalVal = {};
tSBufferInfo g_arrSBufferShareData[(int)eCBUFFER_SBUFFER_SHAREDATA_IDX::END] = {};
Matrix g_matViewProj;

const eCOMPONENT_TYPE g_RenderComIdxStart = eCOMPONENT_TYPE::MESH_RENDER;
const eCOMPONENT_TYPE g_RenderComIdxEnd = eCOMPONENT_TYPE::SCRIPT_HOLDER;



const string g_voidStr = "";

//====================//
//SimpleMath Constants//
//====================//
const struct Vector2 Vector2::Zero(0.f, 0.f);
const struct Vector2 Vector2::One(1.f, 1.f);
const struct Vector2 Vector2::Unit[2] = {Vec2(1.f, 0.f), Vec2(0.f, 1.f)};
//const struct Vector2 Vector2::UnitX(1.f, 0.f);
//const struct Vector2 Vector2::UnitY(0.f, 1.f);

const struct Vector3 Vector3::Zero(0.f, 0.f, 0.f);
const struct Vector3 Vector3::One(1.f, 1.f, 1.f);

const struct Vector3 Vector3::Unit[3] = { 
	Vec3(1.f, 0.f, 0.f), 
	Vec3(0.f, 1.f, 0.f), 
	Vec3(0.f, 0.f, 1.f) 
};
//const struct Vector3 Vector3::UnitX(1.f, 0.f, 0.f);
//const struct Vector3 Vector3::UnitY(0.f, 1.f, 0.f);
//const struct Vector3 Vector3::UnitZ(0.f, 0.f, 1.f);

const struct Vector3 Vector3::Up(0.f, 1.f, 0.f);
const struct Vector3 Vector3::Down(0.f, -1.f, 0.f);
const struct Vector3 Vector3::Right(1.f, 0.f, 0.f);
const struct Vector3 Vector3::Left(-1.f, 0.f, 0.f);
const struct Vector3 Vector3::Forward(0.f, 0.f, 1.f);
const struct Vector3 Vector3::Front(0.f, 0.f, 1.f);
const struct Vector3 Vector3::Backward(0.f, 0.f, -1.f);

const struct Vector4 Vector4::Zero(0.f, 0.f, 0.f, 0.f);
const struct Vector4 Vector4::One(1.f, 1.f, 1.f, 1.f);

const struct Vector4 Vector4::Unit[4] = {
	Vec4(1.f, 0.f, 0.f, 0.f),
	Vec4(0.f, 1.f, 0.f, 0.f),
	Vec4(0.f, 0.f, 1.f, 0.f),
	Vec4(0.f, 0.f, 0.f, 1.f)
};
//const struct Vector4 Vector4::UnitX(1.f, 0.f, 0.f, 0.f);
//const struct Vector4 Vector4::UnitY(0.f, 1.f, 0.f, 0.f);
//const struct Vector4 Vector4::UnitZ(0.f, 0.f, 1.f, 0.f);
//const struct Vector4 Vector4::UnitW(0.f, 0.f, 0.f, 1.f);

const struct Matrix Matrix::Identity(
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
);

const struct Matrix Matrix::Zero(
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f
);


const struct Quaternion Quaternion::Identity(0.f, 0.f, 0.f, 1.f);
