#include "pch.h"

#include "cCom_Renderer_TilemapComplete.h"

#include "cResMgr.h"
#include "cMaterial.h"

cCom_Renderer_TilemapComplete::cCom_Renderer_TilemapComplete()
	: ITilemapBase(eTILEMAP_TYPE::COMPLETE)
{
	SetMaterial(cResMgr::GetInst()->FindRes<cMaterial>(strKey_RES_DEFAULT::MATERIAL::TILEMAP_COMPLETE));
}

cCom_Renderer_TilemapComplete::~cCom_Renderer_TilemapComplete()
{
}
