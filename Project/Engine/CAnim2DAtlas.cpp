#include "pch.h"
#include "CAnim2DAtlas.h"

#include "CResMgr.h"
#include "CTexture.h"

#include "CStructBuffer.h"

CAnim2DAtlas::CAnim2DAtlas()
	: CRes(eRES_TYPE::ANIM2D_SPRITE)
{
}

CAnim2DAtlas::~CAnim2DAtlas()
{
}

void CAnim2DAtlas::SetAtlasTexture(Ptr<CTexture> _AtlasTex)
{
	m_AtlasTex = _AtlasTex;

	
}


void CAnim2DAtlas::SetNewAnimUV(vector<tAnimFrameUV>& _vecFrameUV)
{
	std::swap(m_vecFrameUV, _vecFrameUV);
}

void CAnim2DAtlas::SetNewAnimUV(UINT _uNumCol, UINT _uNumRow)
{
	if (nullptr == m_AtlasTex)
		return;

	m_vecFrameUV.clear();

	float RowSliceUV = 1.f / (float)_uNumRow;
	float ColSliceUV = 1.f / (float)_uNumCol;

	//Left Top부터 열 순서대로 저장
	for (UINT Col = 0; Col < _uNumCol; ++Col)
	{
		for (UINT Row = 0; Row < _uNumRow; ++Row)
		{
			tAnimFrameUV uv = {};
			uv.LeftTopUV.x = ColSliceUV * Col;
			uv.LeftTopUV.y = RowSliceUV * Row;

			uv.SliceUV = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}
}



void CAnim2DAtlas::AddAnim2D(const string& _strAnimKey, const tAnimFrameIdx& _vecAnimFrameIdx)
{
	m_mapAnim.insert(make_pair(_strAnimKey, _vecAnimFrameIdx));
}

const tAnimFrameIdx* CAnim2DAtlas::FindAnim2D(const string& _AnimIdxStrKey)
{
	const auto& iter = m_mapAnim.find(_AnimIdxStrKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return &(iter->second);
}


