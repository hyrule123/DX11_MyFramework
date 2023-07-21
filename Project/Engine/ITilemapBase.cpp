#include "pch.h"
#include "ITilemapBase.h"

#include "cGameObject.h"

#include "cResMgr.h"
#include "strKey_Default.h"

#include "jsoncpp.h"

ITilemapBase::ITilemapBase(eTILEMAP_TYPE _eTilemapType)
	: m_TilemapType(_eTilemapType)
	, m_uTileCountX()
	, m_uTileCountY()
	, m_uNumTiles()
{
	SetMesh(cResMgr::GetInst()->FindRes<cMesh>(strKey_RES_DEFAULT::MESH::RECT));
}

ITilemapBase::~ITilemapBase()
{
}

ITilemapBase::ITilemapBase(ITilemapBase const& _other)
	: IRenderer(_other)
	, m_TilemapType(_other.m_TilemapType)
	, m_uTileCountX(_other.m_uTileCountX)
	, m_uTileCountY(_other.m_uTileCountY)
{
}


bool ITilemapBase::SaveJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
	{
		return false;
	}
	else if (false == IRenderer::SaveJson(_pJVal))
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

bool ITilemapBase::LoadJson(Json::Value* _pJVal)
{
	if (nullptr == _pJVal)
	{
		return false;
	}
	else if (false == IRenderer::LoadJson(_pJVal))
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

void ITilemapBase::Init()
{
	//무조건 VP 사용
	GetOwner()->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, true);
}

void ITilemapBase::FinalTick()
{
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_TILEMAP_INT_SIZE_X, &m_uTileCountX);
	GetOwner()->SetMtrlScalarParam(MTRL_SCALAR_TILEMAP_INT_SIZE_Y, &m_uTileCountY);
}

eRENDER_RESULT ITilemapBase::Render()
{
	//true 반환해서 인스턴싱 필요없다고 전달
	if (nullptr == GetMesh() || nullptr == GetCurMaterial())
		return eRENDER_RESULT::FAIL;

	//타일맵의 재질에 변수를 대입한 후 바인딩
	GetCurMaterial()->BindData();

	BindMtrlScalarDataToCBuffer();

	//렌더링 진행
	GetMesh()->Render();

	//드로우콜이 발생했으므로 true 반환
	return eRENDER_RESULT::RENDERED;
}


