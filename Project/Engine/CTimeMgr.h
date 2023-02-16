#pragma once

#include <random>

class CTimeMgr
	: public CSingleton<CTimeMgr>
{	
	SINGLETON(CTimeMgr);

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

	std::mt19937	m_RandomEngine;
	

public:
	float GetDeltaTime() { return m_fDeltaTime; }
	LONGLONG GetCurCount() { return m_llCurCount.QuadPart; }

	UINT32 GetRandom();
};

