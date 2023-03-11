#pragma once

#include "global.h"

namespace RELATIVE_PATH
{
	constexpr const char8_t* u8strContent = u8".\\Content\\";
	constexpr const char* aContent = ".\\Content\\";
	constexpr const wchar_t* wContent = L".\\Content\\";
}

class CPathMgr
	: public CSingleton<CPathMgr>
{	
	SINGLETON(CPathMgr);
private:
	
	wstring	m_u8ProgramPath;
	wstring	m_u8ContentPath;


	//wchar_t		m_szProgramPath[MAX_PATH];
	//wchar_t		m_szContentPath[MAX_PATH];
	//wchar_t		m_szShaderPath[MAX_PATH];
	

public:
	void init();
	
public:
	const wstring& GetContentPath() const { return m_u8ContentPath; }
};

