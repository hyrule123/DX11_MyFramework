#include "pch.h"

#include "cTimeMgr.h"

#include "cEngine.h"

#include "struct.h"


cTimeMgr::cTimeMgr()
	: m_llPrevCount{}
	, m_llCurCount{}
	, m_llFrequency{}
	, m_iCallCount(0)
	, m_fDeltaTime(0.f)
	, m_fTime(0.f)
{
}

cTimeMgr::~cTimeMgr()
{
}



void cTimeMgr::Init()
{
	// 1초당 카운팅 증가량
	QueryPerformanceFrequency(&m_llFrequency);
		
	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void cTimeMgr::Tick()
{	
	QueryPerformanceCounter(&m_llCurCount);

	// tick 간격 시간
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	if (0.5f < m_fDeltaTime)
	{
		m_fDeltaTime = 0.1f;
	}

	// 누적 시간
	m_fTime += m_fDeltaTime;

	//GPU로 보낼 System Value에도 deltatime 등록
	g_GlobalVal.fDeltaTime = m_fDeltaTime;
	g_GlobalVal.fAccTime += m_fDeltaTime;

	// 함수 호출 횟수
	++m_iCallCount;

	// 이전 카운트 값을 현재 카운트로 갱신
	m_llPrevCount = m_llCurCount;		
}

void cTimeMgr::Render()
{
	// 1초에 한번
	if (1.f <= m_fTime)
	{
		wchar_t szBuff[256] = {};
		swprintf_s(szBuff, L"FPS : %d, DELTA_TIME : %f", m_iCallCount, m_fDeltaTime);
		SetWindowText(cEngine::GetInst()->GetMainWnd(), szBuff);

		m_fTime = 0.f;
		m_iCallCount = 0;
	}
}

