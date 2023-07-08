#pragma once

//트리 관리자 클래스
#include "CDirTreeNode.h"
class CDirTree
{
public:
	CDirTree();
	~CDirTree();

	//시스템 탐색
	HRESULT SearchRecursive(stdfs::path const& _RootPath, tDirTreeFilters const& _Filter);

	//파일 명을 헤더 파일에 등록(편의용도)
	template <typename T>
	HRESULT CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName, bool _bEraseExtension, stdfs::path const& _RootNamespace);

	//UserClassInitializer에 자신의 클래스를 등록하는 코드를 생성하는 함수
	template <typename T>
	HRESULT CreateCShaderCode(stdfs::path const& _DirPath, stdfs::path const& _FileName);

	template <typename T>
	HRESULT CreateGShaderStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName);

	template <typename T>
	HRESULT CreateScriptCode(stdfs::path const& _DirPath, stdfs::path const& _FileName);


private:
	CDirTreeNode m_RootDir;

private:
	template <typename T>
	HRESULT OpenWriter(stdfs::path const& _DirPath, stdfs::path const& _FileName, CCodeWriter<T>& _Writer);

	HRESULT DetectNewShaderGroup(std::unordered_map<stdfs::path, tShaderGroup> const& _umapGSGroup);
};











template<typename T>
inline HRESULT CDirTree::CreateStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName, bool _bEraseExtension, stdfs::path const& _RootNamespace)
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

	std::basic_string<T> strCode = NEW_T_STRING(T, "namespace strKey_");

	if (_RootNamespace.empty())
		strCode += _DirPath.filename().string<T>();
	else
		strCode += _RootNamespace.string<T>();

	Writer.WriteCode(strCode);

	hr = m_RootDir.WriteStrKeyTree(Writer, _bEraseExtension);

	if (FAILED(hr))
		return hr;

	return Writer.SaveAll();
}



template<typename T>
inline HRESULT CDirTree::CreateCShaderCode(stdfs::path const& _DirPath, stdfs::path const& _FileName)
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
#include "strKey_CShader.h"

#include <Engine/CUserClassMgr.h>
#include <Engine/CComputeShader.h>)")
);
		Writer.WriteCode(strCode);


		Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::define_T_Constructor_CS));
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

	vector<stdfs::path> vecCSFilePath;
	//노드를 순회돌면서 이름을 정리시킨다.
	m_RootDir.GetAllFiles(vecCSFilePath, false);


	//순회를 돌면서 각 버퍼에 코드 작성
	//0번 버퍼: include
	//1번 버퍼: 클래스 생성
	for (size_t i = 0; i < vecCSFilePath.size(); ++i)
	{
		//0번 버퍼에 include 작성
		{
			const std::basic_string<T>& FileName = vecCSFilePath[i].filename().string<T>();

			std::basic_string<T> strCode;
			strCode += T_PRESET_STR(T, define_Preset::Keyword::IncludeBegin);
			strCode += FileName;
			strCode += T_STRING(T, "\"");
			Writer.WriteCode(strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::basic_string<T>& ClassName = vecCSFilePath[i].filename().replace_extension("").string<T>();
			

			std::basic_string<T> strCode;
			strCode += T_PRESET_STR(T, define_Preset::Keyword::T_Constructor);
			strCode += ClassName;
			strCode += T_STRING(T, ");");
			Writer.WriteCode(strCode, 1);
		}
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

	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::Head));
	Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::DefineSTRKEY));

	{
		std::basic_string<T> strCode(T_STRING(T,
			R"(#include "pch.h"
#include "UserClassInitializer.h"

#include "strKey_Script.h"

#include <Engine/CUserClassMgr.h>
#include <Engine/CScript.h>)")
);
		Writer.WriteCode(strCode);

		Writer.WriteCode(T_PRESET_STR(T, define_Preset::Keyword::define_T_Constructor_Script));
		Writer.WriteCode();
		Writer.WriteCode(T_STRING(T, "//Script Classes"));
	}

	//1번 버퍼에 Script 생성 코드를 작성
	{
		Writer.WriteCode("", 1);
		std::basic_string<T> strCode(T_STRING(T, "void UserClassInitializer::InitScript()"));

		Writer.WriteCode(strCode, 1);
		Writer.OpenBracket(1);
		Writer.WriteCode(T_STRING(T, "CUserClassMgr* pMgr = CUserClassMgr::GetInst();"), 1);
	}

	vector<stdfs::path> vecCSFilePath;
	//노드를 순회돌면서 이름을 정리시킨다.
	m_RootDir.GetAllFiles(vecCSFilePath, false);

	//순회를 돌면서 각 버퍼에 코드 작성
	//0번 버퍼: include
	//1번 버퍼: 클래스 생성
	for (size_t i = 0; i < vecCSFilePath.size(); ++i)
	{
		//0번 버퍼에 include 작성
		{
			const std::basic_string<T>& FileName = vecCSFilePath[i].filename().string<T>();

			std::basic_string<T> strCode;
			strCode += T_PRESET_STR(T, define_Preset::Keyword::IncludeBegin);
			strCode += FileName;
			strCode += T_STRING(T, "\"");
			Writer.WriteCode(strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::basic_string<T>& ClassName = vecCSFilePath[i].filename().replace_extension("").string<T>();

			std::basic_string<T> strCode;
			strCode += T_PRESET_STR(T, define_Preset::Keyword::T_Constructor);
			strCode += ClassName;
			strCode += T_STRING(T, ");");
			Writer.WriteCode(strCode, 1);
		}
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


template<typename T>
inline HRESULT CDirTree::CreateGShaderStrKey(stdfs::path const& _DirPath, stdfs::path const& _FileName)
{
	CCodeWriter<T> Writer;
	HRESULT hr = OpenWriter<T>(_DirPath, _FileName, Writer);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	vector<stdfs::path> vecGSFileName;
	hr = m_RootDir.GetAllFiles(vecGSFileName, false);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	std::unordered_map<stdfs::path, tShaderGroup> umapGSGroup;
	//파일 순회돌면서 그래픽 쉐이더 파일 정리
	for (size_t i = 0; i < vecGSFileName.size(); ++i)
	{
		//string으로 변경
		string strFileName = vecGSFileName[i].string();

		for (int j = 0; j < (int)define_Shader::eGS_TYPE::END; ++j)
		{
			size_t Pos = strFileName.find(RES_INFO::SHADER::GRAPHICS::arrPrefix[j]);
			if (string::npos != Pos)
			{
				//쉐이더별로 지정한 접두사가 있을 경우 해당 접두사를 제거한다.
				strFileName.erase(0, Pos + strlen(RES_INFO::SHADER::GRAPHICS::arrPrefix[j]));

				//파일의 확장자를 json으로 변경하고, 키값으로 사용.
				stdfs::path ShaderGroupName = strFileName;
				ShaderGroupName.replace_extension(RES_INFO::SHADER::Ext_ShaderSetting);

				//쉐이더 그룹명에 각 쉐이더 파이프라인의 데이터를 담은 파일의 이름을 추가한다.
				umapGSGroup[ShaderGroupName].FileName[j] = vecGSFileName[i];

				break;
			}
		}
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
		strCode += T_STRING(T, "GShader");
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
