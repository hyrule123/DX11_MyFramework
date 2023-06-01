#include "pch.h"
#include "CTilemap.h"

#include "CResMgr.h"
#include "strKey_Default.h"

CTilemap::CTilemap(eTILE_TYPE _eTileType)
	: CRenderComponent(eCOMPONENT_TYPE::TILEMAP)
	, m_eTileType(_eTileType)
	, m_uTileCountX(1)
	, m_uTileCountY(1)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(string(strKey_RES_DEFAULT::MESH::RECT)));
}

CTilemap::~CTilemap()
{
}


