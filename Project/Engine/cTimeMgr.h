#pragma once

#include "Singleton.h"

class cTimeMgr
	: public Singleton<cTimeMgr>
{
	SINGLETON(cTimeMgr);

public:
	void init();
	void tick();
	void render();

private:
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_iCallCount;
	float			m_fDeltaTime;
	float			m_fTime;
	
	
public:
	float GetDeltaTime() { return m_fDeltaTime; }
	LONGLONG GetCurCount() { return m_llCurCount.QuadPart; }
};

