#include "pch.h"
#include "CTilemap.h"

#include "CStructBuffer.h"

#include "CResMgr.h"

#include "CTransform.h"

#include "strKeys.h"

CTilemap::CTilemap()
	: CRenderComponent(eCOMPONENT_TYPE::TILEMAP)
	, m_iTileCountX(1)
	, m_iTileCountY(1)
	, m_SBuffer()
{

}

CTilemap::~CTilemap()
{
	DESTRUCTOR_DELETE(m_SBuffer);
}

void CTilemap::init()
{
	switch (m_eTileType)
	{
	case eTILE_TYPE::NOT_SET:
		//init 시점까지 타일맵 종류가 세팅되지 않았을 경우 에러를 발생시킨다.
		assert(eTILE_TYPE::NOT_SET != m_eTileType);
		break;


	case eTILE_TYPE::COMPLETE:

		break;


	case eTILE_TYPE::ATLAS:
	{
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(RESOURCE::MESH::RECT));
		SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(RESOURCE::MATERIAL::TILEMAP_ATLAS));

		UINT Target = eSHADER_PIPELINE_STAGE::__VERTEX | eSHADER_PIPELINE_STAGE::__PIXEL;
		m_SBuffer = new CStructBuffer(tSBufferDesc{ eSTRUCT_BUFFER_TYPE::READ_ONLY, Target, eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE, e_t_SBUFFER_TILE, e_u_UAV_NONE });
		m_SBuffer->Create(sizeof(tTile), m_iTileCountX * m_iTileCountY, nullptr, 0u);
	}
		break;
	default:
		break;
	}


}

void CTilemap::finaltick()
{
}

bool CTilemap::render(eCAMERA_INDEX _eCamIdx)
{
	//true 반환해서 인스턴싱 필요없다고 전달
	if (nullptr == GetMesh() || nullptr == GetCurMaterial())
		return false;

	//이번에 출력될 카메라 인덱스를 자신의 Scalar Data에 등록
	CGameObject* pOwner = GetOwner();
	pOwner->SetMtrlScalarParam(MTRL_SCALAR_INT_CAMIDX, &_eCamIdx);

	//자신의 구조화버퍼 업데이트
	BindData();

	//타일맵의 재질에 변수를 대입한 후 바인딩
	CMaterial* pMtrl = GetCurMaterial().Get();

	//트랜스폼 업데이트 시켜줌
	//Transform()->UpdateData();

	pOwner->SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &m_iTileCountX);
	pOwner->SetMtrlScalarParam(eMTRLDATA_PARAM_SCALAR::INT_1, &m_iTileCountY);

	pMtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());
	pMtrl->BindData();

	//렌더링 진행
	GetMesh()->render();

	//드로우콜이 발생했으므로 false 반환
	return false;
}

void CTilemap::BindData()
{
	m_SBuffer->UploadData(m_vecTile.data(), (UINT)(sizeof(tTile) * m_vecTile.size()));
	m_SBuffer->BindBufferSRV();
}

void CTilemap::SetTileType(eTILE_TYPE _eType)
{
	m_eTileType = _eType;

	m_vecTile.clear();


}

void CTilemap::SetTileCount(UINT _iXCount, UINT _iYCount)
{
	m_iTileCountX = _iXCount;
	m_iTileCountY = _iYCount;


	m_vecTile.clear();
	m_vecTile.resize(m_iTileCountX * m_iTileCountY);

	if (m_SBuffer->GetCapacity() < m_vecTile.size())
	{
		m_SBuffer->Create(sizeof(tTile), (UINT)m_vecTile.size(), nullptr, 0u);
	}

	// 타일 세팅 테스트
	for (size_t i = 0; i < m_iTileCountY; ++i)
	{
		for (size_t j = 0; j < m_iTileCountX; ++j)
		{
			m_vecTile[i * m_iTileCountX + j].vLeftTop.x = m_vSliceSize.x * j;
			m_vecTile[i * m_iTileCountX + j].vLeftTop.y = 0.f;
			m_vecTile[i * m_iTileCountX + j].vSlice = m_vSliceSize;
		}
	}

	m_vecTile[0].vLeftTop = Vec2(m_vSliceSize.x * 7.f, m_vSliceSize.y * 5.f);
}
