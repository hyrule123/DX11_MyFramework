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

void CAnim2DAtlas::SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal)
{
	if (nullptr == m_AtlasTex)
		return;

	m_vecFrameUV.clear();

	float RowSliceUV = 1.f / (float)_uRowTotal;
	float ColSliceUV = 1.f / (float)_uColTotal;

	//Left Top부터 열 순서대로 저장
	for (UINT Col = 0; Col < _uColTotal; ++Col)
	{
		for (UINT Row = 0; Row < _uRowTotal; ++Row)
		{
			tAnimFrameUV uv = {};
			uv.LeftTopUV.x = ColSliceUV * Col;
			uv.LeftTopUV.y = RowSliceUV * Row;

			uv.SliceUV = Vec2(ColSliceUV, RowSliceUV);

			m_vecFrameUV.push_back(uv);
		}
	}
}

void CAnim2DAtlas::SetNewAnimUV(UINT _uColTotal, UINT _uRowTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch)
{
	if (nullptr == m_AtlasTex)
		return;

	UINT colend = _uColStart + _uColPitch - 1u;
	UINT rowend = _uRowStart + _uRowPitch - 1u;

	//조건 확인
	assert(
		0u < _uColPitch
		&& 0u < _uRowPitch
		&& _uColStart <= colend
		&& _uRowStart <= rowend
		&& colend <= _uColTotal
		&& rowend <= _uRowTotal
	);
	
	m_vecFrameUV.clear();

	//슬라이스를 구해준다
	float RowSliceUV = 1.f / (float)_uRowTotal;
	float ColSliceUV = 1.f / (float)_uColTotal;

	//Left Top부터 열 순서대로 저장
	for (UINT Col = _uColStart; Col <= colend; ++Col)
	{
		for (UINT Row = _uRowStart; Row <= rowend; ++Row)
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

void CAnim2DAtlas::AddAnim2D(const string& _strAnimKey, UINT _uColTotal, UINT _uColStart, UINT _uColPitch, UINT _uRowStart, UINT _uRowPitch)
{
	UINT _uRowTotal = (UINT)m_vecFrameUV.size() % _uColTotal;

	UINT colend = _uColStart + _uColPitch - 1u;
	UINT rowend = _uRowStart + _uRowPitch - 1u;

	//에러 검사 - 행의 수가 나눠 떨어져야 함
	assert(
		0u == _uRowTotal
		&& 0u < _uColPitch
		&& 0u < _uRowPitch
		&& _uColStart <= colend
		&& _uRowStart <= rowend
	);

	_uRowTotal = (UINT)m_vecFrameUV.size() / _uColTotal;

	tAnimFrameIdx Anim = {};

	for (UINT Col = _uColStart; Col <= colend; Col++)
	{
		for (UINT Row = _uRowStart; Row <= rowend; Row++)
		{
			Anim.vecFrame.push_back(tAnimFrame{ (Col * _uRowTotal + Row) , });
		}
	}

	Anim.uNumFrame = (UINT)Anim.vecFrame.size();
	Anim.fFullPlayTime = 10.f;
	Anim.strAnimName = _strAnimKey;
	Anim.fTimePerFrame = (float)Anim.fFullPlayTime / (float)Anim.uNumFrame;

	m_mapAnim.insert(make_pair(_strAnimKey, Anim));
}

const tAnimFrameIdx* CAnim2DAtlas::FindAnim2D(const string& _AnimIdxStrKey)
{
	const auto& iter = m_mapAnim.find(_AnimIdxStrKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return &(iter->second);
}


