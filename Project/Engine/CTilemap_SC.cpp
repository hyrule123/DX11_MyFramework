#include "pch.h"
#include "CTilemap_SC.h"

#include "CResMgr.h"
#include "strKey_Default.h"
#include "CCS_SCMapLoader.h"

#include "CTransform.h"

//유닛 로드 및 언로드 용
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "EventDispatcher.h"
#include "CGameObject.h"

using namespace SC_Map;

CTilemap_SC::CTilemap_SC()
	: CTilemap(eTILE_TYPE::COMPLETE)
	, m_tMapData()
	, m_bMapLoaded()
	, m_bUnitLoaded()
	, m_funcLoadUnit()
{
	//메쉬는 부모 클래스에서 설정했음.
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(string(strKey_RES_DEFAULT::MATERIAL::TILEMAP_SC));
	SetMaterial(pMtrl);
}

CTilemap_SC::~CTilemap_SC()
{
	
}


void CTilemap_SC::tick()
{
	if (m_bMapLoaded && false == m_bUnitLoaded)
	{
		m_bUnitLoaded = true;

		//로드 함수 호출
		if (m_funcLoadUnit)
			m_funcLoadUnit(m_tMapData);
	}
}

bool CTilemap_SC::render()
{
	if (false == m_bMapLoaded)
		return true;
	
	//이번에 출력될 카메라 인덱스를 자신의 Scalar Data에 등록
	CGameObject* pOwner = GetOwner();

	//메쉬와 재질 둘 중 하나라도 없을 경우 아예 여기에 들어오지 않으므로 따로 검사해 줄 필요 없음.
	CMesh* pmesh = GetMesh().Get();
	CMaterial* pmtrl = GetCurMaterial().Get();


	//재질에 자신의 Mtrl Scalar Data를 등록해 놓고 만약 정점 갯수가 6개(Rect )보다 많다면
	//플래그를 켜주고 WVP 행렬 형태로 전송한다.
	if (6 < pmesh->GetIdxBufferCount())
	{
		pOwner->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, false);

		int camIdx = pOwner->GetMtrlScalarParam_Int(MTRL_SCALAR_INT_CAMIDX);
		Matrix matWVP = pOwner->GetMtrlScalarParam_Matrix(MTRL_SCALAR_MAT_WORLD);
		matWVP *= g_matCam[camIdx].matVP;
		pOwner->SetMtrlScalarParam(MTRL_SCALAR_MAT_WVP, matWVP.m);
	}
	else
		pOwner->SetMtrlScalarParam_IntFlag(MTRL_SCALAR_STD2D_FLAG, (int)eMTRL_SCALAR_STD2D_FLAG::USE_VP, true);

	pmtrl->BindData();

	BindMtrlScalarDataToCBuffer();

	// 메쉬 그리기 명령

	pmesh->render();

	return true;
}

bool CTilemap_SC::LoadMap(const string& _strMapName)
{
	if (true == m_bMapLoaded && _strMapName == m_tMapData.strMapName)
		return true;

	if (m_bUnitLoaded)
	{
		assert(m_funcUnloadUnit);
		m_funcUnloadUnit();
	}
	m_bUnitLoaded = false;
	

	Ptr<CCS_SCMapLoader> pLoader = CResMgr::GetInst()->FindRes<CComputeShader>(string(strKey_RES_DEFAULT::SHADER::COMPUTE::SCMAPLOADER));

	//기존 리소스가 있을 시 제거 요청
	if (nullptr != m_tMapData.pMapTex)
	{
		m_tMapData.pMapTex = nullptr;
		GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, nullptr);
		CResMgr::GetInst()->DeleteRes<CTexture>(m_tMapData.strMapName);
	}
		
	//데이터 초기화
	m_tMapData = tMapData();

	m_bMapLoaded = pLoader->LoadMap(_strMapName, m_tMapData);

	if (true == m_bMapLoaded)
	{
		GetCurMaterial()->SetTexParam(eMTRLDATA_PARAM_TEX::_0, m_tMapData.pMapTex);

		CTransform* pTF = Transform();

		g_GlobalVal.fMapSizeX = (float)m_tMapData.uNumMegatileX * 32.f;
		g_GlobalVal.fMapSizeY = (float)m_tMapData.uNumMegatileY * 32.f;
		
		pTF->SetSize(Vec3(g_GlobalVal.fMapSizeX, g_GlobalVal.fMapSizeY, 1.f));


		//스타크래프트 맵의 좌표는 Left Top이 원점이므로
		//맵의 좌측 상단을 원점으로 맞춰준다(2사분면에 집어넣어준다)
		//y축만 음수로 뒤집으면 되니까 좌표 맞추기가 편해짐
		Vec2 MapPos = Vec2(g_GlobalVal.fMapSizeX, -g_GlobalVal.fMapSizeY);
		MapPos /= 2.f;
		
		pTF->SetRelativePosXY(MapPos);
	}

	return m_bMapLoaded;
}




