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
	//애니메이션 재생 종료 시 return
	if (true == m_bFinish)
		return;

	//시간 추가
	m_fTime += DELTA_TIME;

	//프레임당 재생시간을 넘어갔을 경우
	if (m_vecFrm[m_iCurFrm].fDuration < m_fTime)
	{
		//ftime에서 한프레임별 시간을 뺴준후
		m_fTime = m_fTime - m_vecFrm[m_iCurFrm].fDuration;

		//다음 프레임으로 진행한다.
		++m_iCurFrm;

		//마지막 프레임일 경우
		if (m_vecFrm.size() <= m_iCurFrm)
		{
			//마지막 프레임으로 고정시킨다
			m_iCurFrm = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}


}



