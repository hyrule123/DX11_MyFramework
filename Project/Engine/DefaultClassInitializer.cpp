#include "pch.h"
#include "DefaultClassInitializer.h"

#include "define.h"

//eCOMPONENT_TYPE	
//TRANSFORM,		// 위치, 크기, 회전
//COLLIDER2D,		// 2차원 충돌
//COLLIDER3D,		// 3차원 충돌
//ANIMATOR2D,		// Sprite Animation
//ANIMATOR3D,		// Bone Sknning Animation
//LIGHT2D,			// 2차원 광원
//LIGHT3D,			// 3차원 광원
//CAMERA,			// cCom_Camera
//
//// render
//RENDERER,
////MESH_RENDER,		// 기본적인 렌더링
////PARTICLE_SYSTEM, // 입자 렌더링
////TILEMAP,		// 2차원 타일
////LANDSCAPE,		// 3차원 지형
////DECAL,			// 내부 렌더링
//
//// custom
//SCRIPT_HOLDER,

#include "cUserClassMgr.h"
#include "components.h"

void DefaultClassInitializer::InitDefaultComponent()
{
	cUserClassMgr* pMgr = cUserClassMgr::GetInst();


}
