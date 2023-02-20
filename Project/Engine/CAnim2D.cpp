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



