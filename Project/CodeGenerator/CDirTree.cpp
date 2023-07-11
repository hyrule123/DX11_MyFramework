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

HRESULT CDirTree::SearchRecursive(stdfs::path const& _RootPath, std::regex const& _regex)
{
	m_RootDir.Clear();

	return m_RootDir.SearchRecursive( _RootPath, _RootPath, _regex);
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

HRESULT CDirTree::CreateStrKeyHeader(stdfs::path const& _FilePath, stdfs::path const& _RootNamespace, bool _bEraseExtension)
{
	CCodeWriter Writer;

	Writer.WriteCode(define_Preset::Keyword::Head::A);

	Writer.WriteCode(define_Preset::Keyword::DefineSTRKEY::A);

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode();
	Writer.WriteCode();

	string strCode = "namespace strKey_";
	strCode += _RootNamespace.string();
	Writer.WriteCode(strCode);

	HRESULT hr = m_RootDir.WriteStrKeyTree(Writer, _bEraseExtension);

	if (FAILED(hr))
		return hr;

	return Writer.SaveAll<char>(_FilePath);
}




HRESULT CDirTree::CreateCShaderCode(stdfs::path const& _FilePath)
{
	CCodeWriter Writer;

	Writer.WriteCode(define_Preset::Keyword::Head::A);
	Writer.WriteCode(define_Preset::Keyword::DefineSTRKEY::A);

	{
		std::string strCode(
R"(#include "pch.h"
#include "UserClassInitializer.h"
#include "strKey_CShader.h"

#include <Engine/UserClassMgr.h>
#include <Engine/CComputeShader.h>)"
);
		Writer.WriteCode(strCode);


		Writer.WriteCode(define_Preset::Keyword::define_T_Constructor_CS::A);
		Writer.WriteCode();
		Writer.WriteCode("//Compute Shader Classes");
	}

	//1번 버퍼에 CS 생성 코드를 작성
	{
		Writer.WriteCode("", 1);
		std::string strCode = "void UserClassInitializer::InitCS()";

		Writer.WriteCode(strCode, 1);
		Writer.OpenBracket(1);
		//Writer.WriteCode("CUserClassMgr* pMgr = CUserClassMgr::GetInst();", 1);
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
			const std::string& FileName = vecCSFilePath[i].filename().string();

			std::string strCode;
			strCode += define_Preset::Keyword::IncludeBegin::A;
			strCode += FileName + ".h";
			strCode += "\"";
			Writer.WriteCode(strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::string& ClassName = vecCSFilePath[i].filename().replace_extension("").string();


			std::string strCode;
			strCode += define_Preset::Keyword::T_Constructor::A;
			strCode += ClassName;
			strCode += ");";
			Writer.WriteCode(strCode, 1);
		}
	}


	Writer.CloseBracket(false, 1);

	HRESULT hr = Writer.SaveAll<char>(_FilePath);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	return S_OK;
}


HRESULT CDirTree::CreateScriptCPP(stdfs::path const& _FilePath)
{
	CCodeWriter Writer;

	Writer.WriteCode(define_Preset::Keyword::Head::A);
	Writer.WriteCode(define_Preset::Keyword::DefineSTRKEY::A);

	{
		std::string strCode(
R"(#include "pch.h"
#include "UserClassInitializer.h"

#include "strKey_Script.h"

#include <Engine/UserClassMgr.h>
#include <Engine/CScript.h>)"
);
		Writer.WriteCode(strCode);

		Writer.WriteCode(define_Preset::Keyword::define_T_Constructor_Script::A);
		Writer.WriteCode();
		Writer.WriteCode("//Script Classes");
	}

	//1번 버퍼에 Script 생성 코드를 작성
	{
		Writer.WriteCode("", 1);
		std::string strCode( "void UserClassInitializer::InitScript()");

		Writer.WriteCode(strCode, 1);
		Writer.OpenBracket(1);
		//Writer.WriteCode("CUserClassMgr* pMgr = CUserClassMgr::GetInst();", 1);
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
			const std::string& FileName = vecCSFilePath[i].filename().string();

			std::string strCode;
			strCode += define_Preset::Keyword::IncludeBegin::A;
			strCode += FileName;
			strCode += ".h";
			strCode +=  "\"";
			Writer.WriteCode(strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::string& ClassName = vecCSFilePath[i].filename().replace_extension("").string();

			std::string strCode;
			strCode += define_Preset::Keyword::T_Constructor::A;
			strCode += ClassName;
			strCode +=  ");";
			Writer.WriteCode(strCode, 1);
		}
	}


	Writer.CloseBracket(false, 1);

	HRESULT hr = Writer.SaveAll<char>(_FilePath);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	return S_OK;

}




HRESULT CDirTree::CreateGShaderStrKey(stdfs::path const& _FilePath)
{
	CCodeWriter Writer;

	vector<stdfs::path> vecGSFileName;
	HRESULT hr = m_RootDir.GetAllFiles(vecGSFileName, false);
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
	Writer.WriteCode(define_Preset::Keyword::Head::A);

	Writer.WriteCode(define_Preset::Keyword::DefineSTRKEY::A);

	Writer.WriteCode();
	Writer.WriteCode();

	{
		std::string strCode;
		strCode +=  "namespace ";
		strCode += define_Preset::Keyword::strKey::A;
		strCode +=  "GShader";
		Writer.WriteCode(strCode);
		Writer.OpenBracket();
	}

	for (const auto& iter : umapGSGroup)
	{
		std::string strCode;
		strCode +=  "STRKEY ";
		strCode += iter.first.filename().replace_extension("").string();
		strCode += " = \"";
		strCode += iter.first.string();
		strCode += "\";";
		Writer.WriteCode(strCode);
	}

	Writer.CloseBracketAll();

	return Writer.SaveAll<char>(_FilePath);
}
