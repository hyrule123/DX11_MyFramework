#pragma once


class CPathMgr
	: public CSingleton<CPathMgr>
{	
	SINGLETON(CPathMgr);
private:
	wchar_t		m_szProgramPath[MAX_PATH];
	wchar_t		m_szContentPath[MAX_PATH];
	wchar_t		m_szShaderPath[MAX_PATH];

public:
	void init();
	
public:
	const wchar_t* GetContentPath() const { return m_szContentPath; }
	const wchar_t* GetShaderPath() const { return m_szShaderPath; }
};

