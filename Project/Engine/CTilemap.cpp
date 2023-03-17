#include "pch.h"
#include "CTilemap.h"

#include "CResMgr.h"
#include "strKeyDefault.h"

CTilemap::CTilemap(eTILE_TYPE _eTileType)
	: CRenderComponent(eCOMPONENT_TYPE::TILEMAP)
	, m_eTileType(_eTileType)
	, m_uTileCountX(1)
	, m_uTileCountY(1)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(string(DEFAULT_RES::MESH::RECT)));
}

CTilemap::~CTilemap()
{
}


