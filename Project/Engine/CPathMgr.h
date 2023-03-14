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
	MAKE_RELATIVE_PATH(SHADER_GRAPHICS, .\\Content\\Shader\\Graphics);
	MAKE_RELATIVE_PATH(SHADER_COMPUTE, .\\Content\\Shader\\Compute);
	MAKE_RELATIVE_PATH(TEXTURE, .\\Content\\Texture);
	MAKE_RELATIVE_PATH(SCMAP, .\\Content\\Maps);
}
namespace DIRECTORY_NAME
{
	MAKE_RELATIVE_PATH(CONTENT, Content);
	MAKE_RELATIVE_PATH(SHADER, Shader);
	MAKE_RELATIVE_PATH(GRAPHICS, Graphics);
	MAKE_RELATIVE_PATH(COMPUTE, Compute);
	MAKE_RELATIVE_PATH(TEXTURE, Texture);
	MAKE_RELATIVE_PATH(SCMAP, Maps);
	MAKE_RELATIVE_PATH(SAVED_SETTINGS, SavedSetting)
}

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

