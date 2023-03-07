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
	//CurrentDirectory�� ���־�Ʃ������� ������Ҷ��� ���α׷����� �����ų�� �ּҰ� �ٸ��� ����
	//GetCurrentDirectory(256, m_szContentPath);

	//���� ���α׷��� �ּҸ� �޾ƿ��°� �� ����.(������ �ּҸ� �޾ƿ� �� ����)
	GetModuleFileNameW(0, m_szProgramPath, 256);

	// 2�ܰ� ���������� ��
	int iLen = (int)wcslen(m_szProgramPath);

	//Bin �ٱ����� ������ OutputFile ������ �ּҸ� �����
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

