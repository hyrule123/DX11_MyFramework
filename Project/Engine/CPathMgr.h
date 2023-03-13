#pragma once

#include "global.h"

#define MAKE_RELATIVE_PATH(_NameSpace, _strPath) \
namespace _NameSpace \
{ \
constexpr const char8_t* U8 = u8#_strPath;\
constexpr const char* A = #_strPath;\
constexpr const wchar_t* W = L#_strPath;\
}

namespace RELATIVE_PATH
{
	MAKE_RELATIVE_PATH(CONTENT, .\\Content);
	MAKE_RELATIVE_PATH(SHADER, .\\Content\\Shader)
}

class CPathMgr
	: public CSingleton<CPathMgr>
{	
	SINGLETON(CPathMgr);
private:
	std::filesystem::path m_ContentPath;
	wstring m_ContentPathW;
	string  m_ContentPathU8;

	//wchar_t		m_szProgramPath[MAX_PATH];
	//wchar_t		m_szContentPath[MAX_PATH];
	//wchar_t		m_szShaderPath[MAX_PATH];
	

public:
	void init();
	
public:
	const wstring& GetContentAbsPathW() const { return m_ContentPathW; }
	const string& GetContentAbsPathU8() const { return m_ContentPathU8; }
};

