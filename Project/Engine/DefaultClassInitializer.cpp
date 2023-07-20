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

#include "cComMgr.h"
#include "components.h"
#include "strKey_Default.h"


#include "cCom_Coll2D_Circle.h"
#include "cCom_Coll2D_OBB.h"
#include "cCom_Coll2D_Point.h"
#include "cCom_Coll2D_Rect.h"

#include "cCom_Light2D.h"
#include "cCom_Camera.h"

#include "cCom_Renderer_Default.h"
#include "cCom_Renderer_ParticleBasic.h"
#include "cCom_Renderer_TilemapAtlas.h"
#include "cCom_Renderer_TilemapComplete.h"



void DefaultClassInitializer::InitDefaultComponent()
{
	cComMgr* pMgr = cComMgr::GetInst();

	pMgr->AddComConstructor<cCom_Coll2D_Circle>(strkey_DefaultCom::cCom_Coll2D_Circle);
	pMgr->AddComConstructor<cCom_Coll2D_OBB>(strkey_DefaultCom::cCom_Coll2D_OBB);
	pMgr->AddComConstructor<cCom_Coll2D_Point>(strkey_DefaultCom::cCom_Coll2D_Point);
	pMgr->AddComConstructor<cCom_Coll2D_Rect>(strkey_DefaultCom::cCom_Coll2D_Rect);

	pMgr->AddComConstructor<cCom_Light2D>(strkey_DefaultCom::cCom_Light2D);
	pMgr->AddComConstructor<cCom_Camera>(strkey_DefaultCom::cCom_Camera);

	pMgr->AddComConstructor<cCom_Renderer_Default>(strkey_DefaultCom::cCom_Renderer_Default);
	pMgr->AddComConstructor<cCom_Renderer_ParticleBasic>(strkey_DefaultCom::cCom_Renderer_ParticleBasic);
	pMgr->AddComConstructor<cCom_Renderer_TilemapAtlas>(strkey_DefaultCom::cCom_Renderer_TilemapAtlas);
	pMgr->AddComConstructor<cCom_Renderer_TilemapComplete>(strkey_DefaultCom::cCom_Renderer_TilemapComplete);

}
