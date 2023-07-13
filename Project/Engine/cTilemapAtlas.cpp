#include "pch.h"

#include "define.h"

#include "cTilemapAtlas.h"

#include "cStructBuffer.h"

#include "cResMgr.h"

#include "cTransform.h"

#include "strKey_Default.h"

cTilemapAtlas::cTilemapAtlas()
	: cTilemap(eTILEMAP_TYPE::ATLAS)
	, m_vSliceSize()
	, m_vecTile()
	, m_SBuffer()
{
	SetMaterial(cResMgr::GetInst()->FindRes<cMaterial>(string(strKey_RES_DEFAULT::MATERIAL::TILEMAP_ATLAS)));

	tSBufferDesc Desc = {};
	Desc.eSBufferType = eSTRUCT_BUFFER_TYPE::READ_ONLY;
	Desc.flag_PipelineBindTarget_SRV = define_Shader::ePIPELINE_STAGE_FLAG::__VERTEX | define_Shader::ePIPELINE_STAGE_FLAG::__PIXEL;
	Desc.REGISLOT_t_SRV = REGISLOT_t_SBUFFER_TILE;
	Desc.REGISLOT_u_UAV = REGISLOT_u_UAV_NONE;

	m_SBuffer = std::make_unique<cStructBuffer>();
	m_SBuffer->SetDesc(Desc);
	m_SBuffer->Create(sizeof(tTile), GetTileCountX() * GetTileCountY(), nullptr, 0u);
}

cTilemapAtlas::~cTilemapAtlas()
{
}

cTilemapAtlas::cTilemapAtlas(cTilemapAtlas const& _other)
	: cTilemap(_other)
	, m_vSliceSize(_other.m_vSliceSize)
	, m_vecTile(_other.m_vecTile)
	, m_SBuffer(_other.m_SBuffer->Clone())
{
}


void cTilemapAtlas::finaltick()
{
}

bool cTilemapAtlas::render()
{
	//자신의 Tile 구조화버퍼를 업데이트 한다.
	BindData();
	return cTilemap::render();
}


void cTilemapAtlas::BindData()
{
	m_SBuffer->UploadData(m_vecTile.data(), (UINT)(sizeof(tTile) * m_vecTile.size()));
	m_SBuffer->BindBufferSRV();
}

void cTilemapAtlas::SetTileCount(UINT _iXCount, UINT _iYCount)
{
	cTilemap::SetTileCount(_iXCount, _iYCount);


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
