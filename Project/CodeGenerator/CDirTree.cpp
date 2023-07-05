#include "pch.h"
#include "CDirTree.h"

#include "MacroFunc.h"

#include "CCodeWriter.h"
#include "CDirTreeNode.h"

#ifndef JSON_DLL
#define JSON_DLL
#include <UtilLib_DLL/json/json.h>
#ifdef _DEBUG
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Debug")
#else
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Release")
#endif
#endif

#include <Engine/CGraphicsShader.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_Debug.lib")
#else
#pragma comment(lib, "Engine/Engine_Release.lib")
#endif


CDirTree::CDirTree()
	: m_RootDir()
{
}

CDirTree::~CDirTree()
{
}

HRESULT CDirTree::SearchRecursive(stdfs::path const& _RootPath, tDirTreeFilters const& _Filter)
{
	m_RootDir.Clear();

	tDirTreeFilters LowerFilter;

	for (size_t i = 0; i < _Filter.vecExtInclude.size(); ++i)
	{
		LowerFilter.vecExtInclude.push_back(MacroFunc::LowerCase<wchar_t>(_Filter.vecExtInclude[i].wstring()));
	}

	for (size_t i = 0; i < _Filter.vecIncludeKeyword.size(); ++i)
	{
		LowerFilter.vecIncludeKeyword.push_back(MacroFunc::LowerCase<wchar_t>(_Filter.vecIncludeKeyword[i].wstring()));
	}

	for (size_t i = 0; i < _Filter.vecExcludeKeyword.size(); ++i)
	{
		LowerFilter.vecExcludeKeyword.push_back(MacroFunc::LowerCase<wchar_t>(_Filter.vecExcludeKeyword[i].wstring()));
	}


	return m_RootDir.SearchRecursive( _RootPath, LowerFilter);
}

HRESULT CDirTree::DetectNewShaderGroup(std::unordered_map<stdfs::path, tShaderGroup> const& _umapGSGroup)
{
	std::vector<std::string> vecNewShaderGroup;

	const static stdfs::path ShaderSettingDir = stdfs::path(define_Preset::Path::Content::A) / stdfs::path(RES_INFO::SHADER::GRAPHICS::DirName);

	//map을 순회 돌아주면서
	for (const auto& iter : _umapGSGroup)
	{
		stdfs::path ShaderFilePath = ShaderSettingDir / iter.first;

		//파일이 존재하지 않으면 json 파일 초기화 및 생성을 해준다.
		if (false == stdfs::exists(ShaderFilePath))
		{
			std::ofstream ofs(ShaderFilePath);
			if (false == ofs.is_open())
			{
				DEBUG_BREAK;
				return ERROR_FILE_INVALID;
			}

			Json::Value jVal;
			{
				CGraphicsShader DummyGS;
				DummyGS.SetKey(iter.first.string());
				if (false == DummyGS.SaveJson(&jVal))
				{
					DEBUG_BREAK;
					return E_FAIL;
				}
			}

			for (int i = 0; i < (int)define_Shader::eGS_TYPE::END; ++i)
			{
				jVal[RES_INFO::SHADER::GRAPHICS::JSON_KEY::arr_strKey_PipeLine[i]] = iter.second.FileName[i].string();
			}

			ofs << jVal;
			ofs.close();

			vecNewShaderGroup.push_back(iter.first.string());
		}
	}

	//새 쉐이더 그룹이 발견되면 어떤 쉐이더 그룹이 새로 만들었는지 
	if (false == vecNewShaderGroup.empty())
	{
		string Message = "New Shader Detected!\n";
		for (size_t i = 0; i < vecNewShaderGroup.size(); ++i)
		{
			Message += vecNewShaderGroup[i];
			Message += "\n";
		}

		MessageBoxA(nullptr, Message.c_str(), nullptr, MB_OK);
	}

	return S_OK;
}

