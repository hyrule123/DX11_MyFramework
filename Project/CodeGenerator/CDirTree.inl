#include "CDirTree.h"

template<typename T>
inline HRESULT CDirTree::CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName, bool _bEraseExtension)
{
	CCodeWriter<T> Writer;
	HRESULT hr = OpenWriter<T>(_DirPath, _FileName, Writer);
	if (FAILED(hr))
		return hr;

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::Head));

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::DefineSTRKEY));

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode();
	Writer.WriteCode();

	hr = m_RootDir.WriteStrKey(Writer, _bEraseExtension);

	if (FAILED(hr))
		return hr;

	return Writer.SaveAll();
}



template<typename T>
inline HRESULT CDirTree::CreateComputerShaderCode(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	CCodeWriter<T> Writer;
	HRESULT hr = OpenWriter<T>(_DirPath, _FileName, Writer);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::Head));
	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::DefineSTRKEY));

	{
		std::basic_string<T> strCode(T_STRING(T,
			R"(#include "pch.h"
#include "UserClassInitializer.h"

#include <Engine/CUserClassMgr.h>
#include <Engine/CComputeShader.h>)"));
		Writer.WriteCode(strCode);


		Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::define_T_Constructor));
		Writer.WriteCode();
		Writer.WriteCode(T_STRING(T, "//Compute Shader Classes"));
	}

	//1번 버퍼에 CS 생성 코드를 작성
	{
		Writer.WriteCode("", 1);
		std::basic_string<T> strCode(T_STRING(T, "void UserClassInitializer::InitCS()"));

		Writer.WriteCode(strCode, 1);
		Writer.OpenBracket(1);
		Writer.WriteCode(T_STRING(T, "CUserClassMgr* pMgr = CUserClassMgr::GetInst();"), 1);
	}

	//노드를 순회돌면서 이름을 정리시킨다.
	hr = m_RootDir.WriteComputeShaderCode<T>(Writer);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}


	Writer.CloseBracket(false, 1);

	hr = Writer.SaveAll();
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	return S_OK;
}

template<typename T>
inline HRESULT CDirTree::CreateScriptCode(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	CCodeWriter<T> Writer;
	HRESULT hr = OpenWriter<T>(_DirPath, _FileName, Writer);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}


}

template<typename T>
inline HRESULT CDirTree::CreateGraphicsShaderStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	CCodeWriter<T> Writer;
	HRESULT hr = OpenWriter<T>(_DirPath, _FileName, Writer);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	//파일 순회돌면서 그래픽 쉐이더 파일 정리
	std::unordered_map<stdfs::path, tShaderGroup> umapGSGroup;
	hr = m_RootDir.WriteGraphicsShaderStrKey(umapGSGroup);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	//새로 생성된 쉐이더 코드가 있는지 체크
	hr = DetectNewShaderGroup(umapGSGroup);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	//코드 작성 시작
	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::Head));

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::DefineSTRKEY));

	Writer.WriteCode();
	Writer.WriteCode();

	{
		std::basic_string<T> strCode;
		strCode += T_STRING(T, "namespace ");
		strCode += T_PRESET_STR(T, define_Preset::Keyword::strKey);
		strCode += T_STRING(T, "GRAPHICS");
		Writer.WriteCode(strCode);
		Writer.OpenBracket();
	}

	for (const auto& iter : umapGSGroup)
	{
		std::basic_string<T> strCode;
		strCode += T_STRING(T, "STRKEY ");
		strCode += iter.first.filename().replace_extension("").string<T>();
		strCode += " = \"";
		strCode += iter.first.string<T>();
		strCode += "\";";
		Writer.WriteCode(strCode);
	}

	Writer.CloseBracketAll();

	return Writer.SaveAll();
}


template<typename T>
inline HRESULT CDirTree::OpenWriter(stdfs::path const& _DirPath, stdfs::path const& _FileName, CCodeWriter<T>& _Writer)
{
	//아직 경로가 준비되지 않았을 경우 시작 X
	if (false == m_RootDir.IsReady())
	{
		DEBUG_BREAK;
		return E_NOT_SET;
	}

	//해당 경로가 존재하지 않을 경우에도 에러 리턴
	else if (false == stdfs::exists(_DirPath))
	{
		DEBUG_BREAK;
		return CO_E_BAD_PATH;
	}

	stdfs::path FullPath = _DirPath / _FileName;

	_Writer.Open(FullPath);
	if (false == _Writer.IsOpen())
	{
		DEBUG_BREAK;
		return ERROR_PATH_NOT_FOUND;
	}

	return S_OK;
}
