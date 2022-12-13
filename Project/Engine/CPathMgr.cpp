#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"

CPathMgr::CPathMgr()
	: m_szContentPath{}
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
	GetModuleFileNameW(0, m_szContentPath, 256);

	// 2�ܰ� ���������� ��
	int iLen = (int)wcslen(m_szContentPath);
	int SlashCount = 0;

	//Bin �ٱ����� ������ OutputFile ������ �ּҸ� �����
	for (int i = iLen - 1; i >= 0; --i)
	{
		if (L'\\' == m_szContentPath[i])
		{
			++SlashCount;

			if (2 == SlashCount)
			{
				m_szContentPath[i] = 0;
				break;
			}
		}
	}

	// + bin\\content
	wcscat_s(m_szContentPath, L"\\bin\\content\\");
}

