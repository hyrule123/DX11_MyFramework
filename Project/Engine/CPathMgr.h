#pragma once

#include "global.h"

#include "strKeyDefault.h"



class CPathMgr
	: public CSingleton<CPathMgr>
{	
	SINGLETON(CPathMgr);
private:
	std::filesystem::path m_ContentPath;

	//wchar_t		m_szProgramPath[MAX_PATH];
	//wchar_t		m_szContentPath[MAX_PATH];
	//wchar_t		m_szShaderPath[MAX_PATH];
	

public:
	void init();
	
public:
	const std::filesystem::path& GetPath() const { return m_ContentPath; }
	//wstring GetContentAbsPathW() const { return m_ContentPath.wstring(); }
	//string GetContentAbsPathU8() const { return m_ContentPath.string(); }
	
};

