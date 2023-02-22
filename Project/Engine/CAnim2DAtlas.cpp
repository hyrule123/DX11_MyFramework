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

void CAnim2DAtlas::SetNewAnimUV(UINT _uNumCol, UINT _uNumRow, UINT _uStartCol, UINT _uEndCol, UINT _uStartRow, UINT _uEndRow)
{
	if (nullptr == m_AtlasTex)
		return;
	//조건 확인
	assert(
		_uStartCol < _uEndCol 
		&& _uStartRow < _uEndRow
		&& _uEndCol <= _uNumCol
		&& _uEndRow <= _uNumRow
	);
	
	m_vecFrameUV.clear();

	//슬라이스를 구해준다
	float RowSliceUV = 1.f / (float)_uNumRow;
	float ColSliceUV = 1.f / (float)_uNumCol;

	//Left Top부터 열 순서대로 저장
	for (UINT Col = _uStartCol; Col < _uEndCol; ++Col)
	{
		for (UINT Row = _uStartRow; Row < _uEndRow; ++Row)
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

std::pair<std::unordered_map<std::string, tAnimFrameIdx>::iterator, bool> CAnim2DAtlas::AddAnim2D(const string& _strAnimKey, UINT _uNumCol, UINT _uStartCol, UINT _uEndCol, UINT _uStartRow, UINT _uEndRow)
{
	UINT _uNumRow = (UINT)m_vecFrameUV.size() % _uNumCol;

	//에러 검사 - 맞아 떨어져야 함
	assert(
		0u == _uNumRow
		&& _uStartCol < _uEndCol
		&& _uStartRow < _uEndRow
	);

	_uNumRow = (UINT)m_vecFrameUV.size() / _uNumCol;

	tAnimFrameIdx Anim = {};

	for (UINT Col = _uStartCol; Col <= _uEndCol; Col++)
	{
		for (UINT Row = _uStartRow; Row <= _uEndRow; Row++)
		{
			Anim.vecFrame.push_back(tAnimFrame{ (Col * _uNumRow + Row) , });
		}
	}

	return m_mapAnim.insert(make_pair(_strAnimKey, Anim));
}

const tAnimFrameIdx* CAnim2DAtlas::FindAnim2D(const string& _AnimIdxStrKey)
{
	const auto& iter = m_mapAnim.find(_AnimIdxStrKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return &(iter->second);
}


