#include "pch.h"
#include "cTilemap.h"

#include "cResMgr.h"
#include "strKey_Default.h"

#include "jsoncpp.h"

cTilemap::cTilemap(eTILEMAP_TYPE _eTilemapType)
	: cRenderComponent(eCOMPONENT_TYPE::TILEMAP)
	, m_TilemapType(_eTilemapType)
	, m_uTileCountX(1)
	, m_uTileCountY(1)
{
	cResMgr* pMgr = cResMgr::GetInst();

	SetMesh(pMgr->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::RECT));
	
	//타일맵 타입에 따른 기본 재질을 설정한다.
	switch (_eTilemapType)
	{
	case eTILEMAP_TYPE::ATLAS:
		SetMaterial(pMgr->Load<cMaterial>(strKey_RES_DEFAULT::MATERIAL::TILEMAP_ATLAS));

		break;
	case eTILEMAP_TYPE::COMPLETE:
		SetMaterial(pMgr->Load<cMaterial>(strKey_RES_DEFAULT::MATERIAL::TILEMAP_COMPLETE));

		break;
	default:
		break;
	}

	

}

cTilemap::~cTilemap()
{
}

cTilemap::cTilemap(cTilemap const& _other)
	: cRenderComponent(_other)
	, m_TilemapType(_other.m_TilemapType)
	, m_uTileCountX(_other.m_uTileCountX)
	, m_uTileCountY(_other.m_uTileCountY)
{
}


bool cTilemap::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
	{
		return false;
	}
	else if (false == cRenderComponent::SaveJson(_pJVal))
	{
		return false;
	}

	Json::Value& jVal = *_pJVal;

	{
		const char* strKey = RES_INFO::PREFAB::COMPONENT::RENDER_COMP::TILEMAP::m_TilemapType;
		jVal[strKey] = (int)m_TilemapType;
	}
	
	return true;
}

bool cTilemap::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
	{
		return false;
	}
	else if (false == cRenderComponent::LoadJson(_pJVal))
	{
		return false;
	}

	Json::Value& jVal = *_pJVal;

	{
		const char* strKey = RES_INFO::PREFAB::COMPONENT::RENDER_COMP::TILEMAP::m_TilemapType;
		if (jVal.isMember(strKey))
		{
			m_TilemapType = (eTILEMAP_TYPE)jVal[strKey].asInt();
		}
	}

	return true;
}

void cTilemap::init()
{
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, true);
}

bool cTilemap::render()
{
	//true 반환해서 인스턴싱 필요없다고 전달
	if (nullptr == GetMesh() || nullptr == GetCurMaterial())
		return true;

	cGameObject* pOwner = GetOwner();

	//타일맵의 재질에 변수를 대입한 후 바인딩
	cMaterial* pMtrl = GetCurMaterial().Get();

	UINT count = GetTileCountX();
	pOwner->SetMtrlScalarParam(MTRL_SCALAR_TILEMAP_INT_SIZE_X, &count);

	count = GetTileCountY();
	pOwner->SetMtrlScalarParam(MTRL_SCALAR_TILEMAP_INT_SIZE_Y, &count);

	pMtrl->BindData();

	BindMtrlScalarDataToCBuffer();

	//렌더링 진행
	GetMesh()->render();

	//드로우콜이 발생했으므로 true 반환
	return true;
}
;

