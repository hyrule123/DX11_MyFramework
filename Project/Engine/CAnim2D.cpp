#include "pch.h"
#include "CAnim2D.h"

#include "CTexture.h"

#include "CTimeMgr.h"

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
	//�ִϸ��̼� ��� ���� �� return
	if (true == m_bFinish)
		return;

	//�ð� �߰�
	m_fTime += DELTA_TIME;

	//�����Ӵ� ����ð��� �Ѿ�� ���
	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		//ftime���� �������Ӻ� �ð��� ������
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;

		//���� ���������� �����Ѵ�.
		++m_iCurFrm;

		//������ �������� ���
		if (m_vecFrm.size() <= m_iCurFrm)
		{
			//������ ���������� ������Ų��
			m_iCurFrm = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}


}

void CAnim2D::Create(const string& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS)
{
	assert(_AtlasTex.Get());

	SetName(_strAnimName);

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

