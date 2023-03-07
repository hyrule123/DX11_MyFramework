#include "pch.h"
#include "CTilemap_Atlas.h"

#include "CStructBuffer.h"

#include "CResMgr.h"

#include "CTransform.h"

#include "strKeys.h"

CTilemap_Atlas::CTilemap_Atlas()
	: CTilemap(eTILE_TYPE::ATLAS)
	, m_vSliceSize()
	, m_vecTile()
	, m_SBuffer()
{
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(RESOURCES::MATERIAL::TILEMAP_ATLAS));

	UINT Target = eSHADER_PIPELINE_STAGE::__VERTEX | eSHADER_PIPELINE_STAGE::__PIXEL;
	m_SBuffer = new CStructBuffer(tSBufferDesc{ eSTRUCT_BUFFER_TYPE::READ_ONLY, Target, eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE, e_t_SBUFFER_TILE, e_u_UAV_NONE });
	m_SBuffer->Create(sizeof(tTile), GetTileCountX() * GetTileCountY(), nullptr, 0u);
}

CTilemap_Atlas::~CTilemap_Atlas()
{
	DESTRUCTOR_DELETE(m_SBuffer);
}


void CTilemap_Atlas::finaltick()
{
}

bool CTilemap_Atlas::render()
{
	//true 반환해서 인스턴싱 필요없다고 전달
	if (nullptr == GetMesh() || nullptr == GetCurMaterial())
		return true;

	CGameObject* pOwner = GetOwner();

	//자신의 구조화버퍼 업데이트
	BindData();

	//타일맵의 재질에 변수를 대입한 후 바인딩
	CMaterial* pMtrl = GetCurMaterial().Get();

	//트랜스폼 업데이트 시켜줌
	//Transform()->UpdateData();

	UINT count = GetTileCountX();
	pOwner->SetMtrlScalarParam(MTRL_SCALAR_TILEMAP_INT_SIZE_X, &count);

	count = GetTileCountY();
	pOwner->SetMtrlScalarParam(MTRL_SCALAR_TILEMAP_INT_SIZE_Y, &count);

	pMtrl->AddMtrlScalarData(pOwner->GetMtrlScalarData());
	pMtrl->BindData();

	//렌더링 진행
	GetMesh()->render();

	//드로우콜이 발생했으므로 true 반환
	return true;
}

void CTilemap_Atlas::BindData()
{
	m_SBuffer->UploadData(m_vecTile.data(), (UINT)(sizeof(tTile) * m_vecTile.size()));
	m_SBuffer->BindBufferSRV();
}

void CTilemap_Atlas::SetTileCount(UINT _iXCount, UINT _iYCount)
{
	CTilemap::SetTileCount(_iXCount, _iYCount);


	m_vecTile.clear();
	m_vecTile.resize(GetTileCountX() * GetTileCountY());

	if (m_SBuffer->GetCapacity() < m_vecTile.size())
	{
		m_SBuffer->Create(sizeof(tTile), (UINT)m_vecTile.size(), nullptr, 0u);
	}

	// 타일 세팅 테스트
	for (size_t i = 0; i < GetTileCountY(); ++i)
	{
		for (size_t j = 0; j < GetTileCountX(); ++j)
		{
			m_vecTile[i * GetTileCountX() + j].vLeftTop.x = m_vSliceSize.x * j;
			m_vecTile[i * GetTileCountX() + j].vLeftTop.y = 0.f;
			m_vecTile[i * GetTileCountX() + j].vSlice = m_vSliceSize;
		}
	}

	m_vecTile[0].vLeftTop = Vec2(m_vSliceSize.x * 7.f, m_vSliceSize.y * 5.f);
}