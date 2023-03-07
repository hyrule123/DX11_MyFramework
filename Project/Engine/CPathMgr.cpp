#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"

CPathMgr::CPathMgr()
	: m_szProgramPath{}
	, m_szContentPath{}
	, m_szShaderPath{}
	
{
}

CPathMgr::~CPathMgr()
{
}


void CPathMgr::init()
{
	//CurrentDirectory는 비주얼스튜디오에서 디버그할때와 프로그램으로 실행시킬때 주소가 다르게 나옴
	//GetCurrentDirectory(256, m_szContentPath);

	//현재 프로그램의 주소를 받아오는게 더 나음.(고정된 주소를 받아올 수 있음)
	GetModuleFileNameW(0, m_szProgramPath, 256);

	// 2단계 상위폴더로 감
	int iLen = (int)wcslen(m_szProgramPath);

	//Bin 바깥으로 나가서 OutputFile 까지의 주소만 남긴다
	for (int i = iLen - 1; i >= 0; --i)
	{
		if (L'\\' == m_szProgramPath[i])
		{
			m_szProgramPath[i] = 0;
			break;
		}
	}

	// + \\content\\
	
	lstrcpyW(m_szContentPath, m_szProgramPath);
	wcscat_s(m_szContentPath, L"\\Content\\");

	lstrcpyW(m_szShaderPath, m_szProgramPath);
	wcscat_s(m_szContentPath, L"\\Shader\\");
}

