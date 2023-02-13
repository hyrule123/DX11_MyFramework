#include "pch.h"
#include "CTilemap.h"

#include "CStructBuffer.h"

#include "CResMgr.h"

#include "CTransform.h"



CTilemap::CTilemap()
	: CRenderComponent(eCOMPONENT_TYPE::TILEMAP)
	, m_iTileCountX(1)
	, m_iTileCountY(1)
	, m_SBuffer()
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>("RectMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>("TilemapMtrl"));

	UINT Target = eSHADER_PIPELINE_STAGE::__VERTEX | eSHADER_PIPELINE_STAGE::__PIXEL;
	m_SBuffer = new CStructBuffer(eSTRUCT_BUFFER_TYPE::READ_ONLY, Target, eCBUFFER_SBUFFER_SHAREDATA_IDX::TILE, e_t_SBUFFER_TILE, e_u_UAV_NONE);
	m_SBuffer->Create(sizeof(tTile), m_iTileCountX * m_iTileCountY, nullptr, 0u);
}

CTilemap::~CTilemap()
{
	if (nullptr != m_SBuffer)
		delete m_SBuffer;
}

void CTilemap::finaltick()
{
}

void CTilemap::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	//Transform에 BindData 요청
	Transform()->UpdateData();

	//자신의 구조화버퍼 업데이트
	BindData();

	//타일맵의 재질에 변수를 대입한 후 바인딩
	CMaterial* pMtrl = GetMaterial().Get();
	pMtrl->SetScalarParam(eMTRLDATA_PARAM_SCALAR::INT_0, &m_iTileCountX);
	pMtrl->SetScalarParam(eMTRLDATA_PARAM_SCALAR::INT_1, &m_iTileCountY);
	pMtrl->BindData();

	//렌더링 진행
	GetMesh()->render();
}

void CTilemap::BindData()
{
	m_SBuffer->UploadData(m_vecTile.data(), (UINT)(sizeof(tTile) * m_vecTile.size()));
	m_SBuffer->BindBufferSRV();
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
