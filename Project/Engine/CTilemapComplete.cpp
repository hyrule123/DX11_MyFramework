#include "pch.h"
#include "CTilemapComplete.h"

#include "CResMgr.h"
#include "strKeyDefaultRes.h"
#include "CCS_SCMapLoader.h"


#include "CTransform.h"

CTilemapComplete::CTilemapComplete()
	: CTilemap(eTILE_TYPE::COMPLETE)
	, m_tMapData()
{
	//메쉬는 부모 클래스에서 설정했음.
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(DEFAULT_RES::MATERIAL::TILEMAP_COMPLETE);
	SetMaterial(pMtrl);
}

CTilemapComplete::~CTilemapComplete()
{
	
}



bool CTilemapComplete::render()
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
	

	pmtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());
	pmtrl->BindData();

	// 메쉬 그리기 명령

	pmesh->render();

	return true;
}

bool CTilemapComplete::LoadMap(const string& _strMapName)
{
	if (true == m_bMapLoaded && _strMapName == m_tMapData.strMapName)
		return true;

	Ptr<CCS_SCMapLoader> pLoader = CResMgr::GetInst()->FindRes<CComputeShader>(DEFAULT_RES::SHADER::COMPUTE::SCMAPLOADER);

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
		Transform()->SetSize(Vec3(m_tMapData.uMapSizeX * 32.f, m_tMapData.uMapSizeY * 32.f, 1.f));
	}

	return m_bMapLoaded;
}

