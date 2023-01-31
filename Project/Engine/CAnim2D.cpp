#include "pch.h"
#include "CAnim2D.h"

#include "CTexture.h"

CAnim2D::CAnim2D()
	: m_pOwner(nullptr)
	, m_iCurFrm()
	, m_bFinish(false)
	, m_fTime()
{
}

CAnim2D::~CAnim2D()
{
}


void CAnim2D::finaltick()
{
	//애니메이션 재생 종료 시 return
	if (true == m_bFinish)
		return;


}

void CAnim2D::Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS)
{
}

