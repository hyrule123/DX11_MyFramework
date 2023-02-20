#include "pch.h"
#include "CAnimMgr.h"

CAnimMgr::CAnimMgr()
{
}

CAnimMgr::~CAnimMgr()
{
}


void CAnimMgr::init()
{
}

void CAnimMgr::Create(const string& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS)
{
	assert(_AtlasTex.Get());



	m_AtlasTex = _AtlasTex;

	Vec2 vResolution = m_AtlasTex->GetSize();

	m_vBackSize = _vBackSize / vResolution;

	for (size_t i = 0; i < _FrameCount; ++i)
	{
		tAnim2DFrm frm = {};

		frm.fDuration = 1.f / (float)_FPS;
		frm.LeftTopUV = Vec2(_vLeftTop.x + _vSlice.x * i, _vLeftTop.y) / vResolution;
		frm.SliceUV = _vSlice / vResolution;

		m_vecFrm.push_back(frm);
	}
}

