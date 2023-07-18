#include "pch.h"
#include "cDirTree.h"

#include <Engine/func.h>
#include <Engine/cGraphicsShader.h>
#include <Engine/cComputeShader.h>

#ifndef JSON_DLL
#define JSON_DLL
#include <UtilLib_DLL/json/json.h>
#ifdef _DEBUG
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Debug")
#else
#pragma comment(lib, "UtilLib_DLL/UtilLib_DLL_Release")
#endif
#endif



#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_Debug.lib")
#else
#pragma comment(lib, "Engine/Engine_Release.lib")
#endif

#include "cCodeWriter.h"
#include "cDirTreeNode.h"


cDirTree::cDirTree()
	: m_RootDir()
{
}

cDirTree::~cDirTree()
{
}

HRESULT cDirTree::SearchRecursive(stdfs::path const& _RootPath, std::regex const& _regex)
{
	m_RootDir.Clear();

	return m_RootDir.SearchRecursive( _RootPath, _RootPath, _regex);
}

HRESULT cDirTree::DetectNewGraphicsShader(std::unordered_map<stdfs::path, tShaderGroup> const& _umapGSGroup)
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
				cGraphicsShader DummyGS;
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

HRESULT cDirTree::DetectNewComputeShader(std::vector<stdfs::path> const& _vecCShader)
{
	std::vector<stdfs::path> vecNewCS;

	const static stdfs::path ShaderDir = stdfs::path(define_Preset::Path::Content::A) / stdfs::path(DIRECTORY_NAME::SHADER_COMPUTE);
	
	const std::regex NumThreadRegex(define_Preset::Regex::Numthread::A);

	//배열을 순회 돌아주면서
	for (size_t i = 0; i < _vecCShader.size(); ++i)
	{
		stdfs::path ShaderFilePath = ShaderDir;
		ShaderFilePath /= _vecCShader[i];
		ShaderFilePath += RES_INFO::SHADER::Ext_ShaderSetting;


		bool bNeedInitialize = true;
		Json::Value CShaderSetting;

		//json 파일이 없을 경우 json 파일을 새로 생성한다.
		if (stdfs::exists(ShaderFilePath))
		{
			//안에 데이터를 가져오고, 데이터가 없을 경우에는 Initialize
			std::ifstream ifs(ShaderFilePath, std::ios::ate);
			if (0 != ifs.tellg())
			{
				ifs.seekg(std::ios::beg);
				ifs >> CShaderSetting;
				bNeedInitialize = false;
			}
		}

		if (bNeedInitialize)
		{
			//json파일을 초기화
			cComputeShader DummyCS;
			DummyCS.SetKey(_vecCShader[i].string());
			if (false == DummyCS.SaveJson(&CShaderSetting))
			{
				DEBUG_BREAK;
				return E_FAIL;
			}

			vecNewCS.push_back(_vecCShader[i]);
		}

		//같은 이름의 HLSL 파일을 열어서 스레드 수를 확인한다.
		stdfs::path HLSLFilePath = define_Preset::Path::HLSL_Proj::A;
		HLSLFilePath /= _vecCShader[i];
		HLSLFilePath += ".hlsl";

		//쉐이더 소스코드를 열어준다.
		std::ifstream ifs(HLSLFilePath);
		if (false == ifs.is_open())
		{
			DEBUG_BREAK;
			return E_FAIL;
		}

		int NumThreads[3]{};
		{
			string FindThread;
			smatch Matches;
			while (false == ifs.eof())
			{
				std::getline(ifs, FindThread);

				size_t pos = FindThread.find("numthreads");
				if (std::string::npos != pos)
				{
					//문자열에서 공백을 제거한다.
					auto iter =
						std::remove_if(
							FindThread.begin(),
							FindThread.end(),
							[](char c)->bool
							{
								return std::isspace(c);
							});

					FindThread.erase(iter, FindThread.end());

					//스레드 갯수를 읽어온다.
					std::smatch Matches;
					if (std::regex_match(FindThread, Matches, NumThreadRegex))
					{
						if (Matches.size() < 4)
						{
							DEBUG_BREAK;
							return E_FAIL;
						}

						try
						{
							for (size_t i = 1; i < Matches.size(); ++i)
							{
								NumThreads[i - 1] = std::stoi(Matches[i]);
							}
						}
						catch (const std::runtime_error& _err)
						{
							_err;
							DEBUG_BREAK;
							return E_FAIL;
						}

						break;
					}
					FindThread.clear();
				}

			}
		}
		ifs.close();

		//하나라도 0일경우 return false
		if (0 == (NumThreads[0] * NumThreads[1] * NumThreads[2]))
		{
			string ErrMsg = "Compute Shader\n";
			ErrMsg += _vecCShader[i].string();
			ErrMsg += "\n thread is Invalid.";
			ERROR_MESSAGE(ErrMsg.c_str());
			return E_FAIL;
		}

		CShaderSetting[JsonKey_cComputeShader::m_arrNumThreads].clear();

		for (int i = 0; i < 3; ++i)
		{
			CShaderSetting[JsonKey_cComputeShader::m_arrNumThreads].append((int)NumThreads[i]);
		}
		

		std::ofstream ofs(ShaderFilePath);
		if (false == ofs.is_open())
		{
			DEBUG_BREAK;
			return E_FAIL;
		}
		ofs << CShaderSetting;
	}

	if (false == vecNewCS.empty())
	{
		string Msg = "New Compute Shader Detected.\n";
		for (size_t i = 0; i < vecNewCS.size(); ++i)
		{
			Msg += vecNewCS[i].string() + "\n";
		}
		MessageBoxA(nullptr, Msg.c_str(), nullptr, MB_OK);
	}

	return S_OK;
}

HRESULT cDirTree::CreateStrKeyHeader(stdfs::path const& _FilePath, stdfs::path const& _RootNamespace, bool _bEraseExtension)
{
	cCodeWriter Writer;

	Writer.WriteCode(0, define_Preset::Keyword::Head::A);

	Writer.WriteCode(0, define_Preset::Keyword::DefineSTRKEY::A);

	//루트 노드의 경우 이름도 알맞게 바꿔줘야 함.
	Writer.WriteCode(0);
	Writer.WriteCode(0);

	string strCode = "namespace strKey_";
	strCode += _RootNamespace.string();
	Writer.WriteCode(0, strCode);

	HRESULT hr = m_RootDir.WriteStrKeyTree(Writer, _bEraseExtension);

	if (FAILED(hr))
		return hr;

	return Writer.SaveAll<char>(_FilePath);
}




HRESULT cDirTree::CreateUserClassInitCode(tAddBaseClassDesc const& _Desc)
{
	cCodeWriter Writer;

	Writer.WriteCode(0, define_Preset::Keyword::Head::A);
	

	{
		std::string strCode(
			R"(#include "pch.h"
#include "UserClassInitializer.h")");
		Writer.WriteCode(0, strCode);

	}
	{

		std::string strCode = "#include \"";
		strCode += _Desc.IncludeStrKeyHeaderName;
		strCode += "\"";
		Writer.WriteCode(0, strCode);
	}

	Writer.WriteCode(0, R"(#include <Engine/cUserClassMgr.h>))");

	{
		std::string strCode = "#include <Engine/";
		strCode += _Desc.IncludeBaseHeaderName;
		strCode += ">";

		Writer.WriteCode(0, strCode);
		Writer.WriteCode(0);
	}
	

	//1번 버퍼에 CS 생성 코드를 작성
	{
		Writer.WriteCode(1, "");

		{
			//CONSTRUCTOR_T(T) 
			std::string strCode = define_Preset::Keyword::Define_Constructor_T::A;
			strCode += _Desc.Constructor_T_MacroDefine;
			Writer.WriteCode(1, strCode);

			Writer.WriteCode(1, "");
		}

		std::string strCode = "void UserClassInitializer::";
		strCode += _Desc.UserClassMgr_InitFuncName;

		Writer.WriteCode(1, strCode);
		Writer.OpenBracket(1);
		//Writer.WriteCode("cUserClassMgr* pMgr = cUserClassMgr::GetInst();", 1);
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
			Writer.WriteCode(0, strCode);
		}

		//1번 버퍼에 클래스 생성 코드 작성
		{
			const std::string& ClassName = vecCSFilePath[i].filename().replace_extension("").string();

			std::string strCode;
			strCode += define_Preset::Keyword::Constructor_T::A;
			strCode += ClassName;
			strCode += ");";
			Writer.WriteCode(1, strCode);
		}
	}


	Writer.CloseBracket(1, false);

	HRESULT hr = Writer.SaveAll<char>(_Desc._FilePath);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	return S_OK;
}





HRESULT cDirTree::CreateShaderStrKey(stdfs::path const& _FilePath)
{
	cCodeWriter Writer;

	vector<stdfs::path> vecShaderFileName;
	HRESULT hr = m_RootDir.GetAllFiles(vecShaderFileName, false);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	std::unordered_map<stdfs::path, tShaderGroup> umapGSGroup;
	std::vector<stdfs::path> vecCS;

	std::regex CSRegex(define_Preset::Regex::CShaderRegex::A);
	//파일 순회돌면서 그래픽 쉐이더 파일 정리
	for (size_t i = 0; i < vecShaderFileName.size(); ++i)
	{
		//string으로 변경
		string strFileName = vecShaderFileName[i].string();

		bool bIsGS = false;
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
				umapGSGroup[ShaderGroupName].FileName[j] = vecShaderFileName[i];

				bIsGS = true;

				break;
			}
		}

		//그래픽 쉐이더가 아닐 경우 컴퓨트쉐이더인지 검사
		if (false == bIsGS)
		{
			//컴퓨트쉐이더일 경우 컴퓨트쉐이더 벡터에 값을 추가
			if(std::regex_search(strFileName, CSRegex))
			{
				vecCS.push_back(vecShaderFileName[i].replace_extension());
			}
		}
	}

	//새로 생성된 그래픽스 쉐이더 코드가 있는지 체크
	hr = DetectNewGraphicsShader(umapGSGroup);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}
	hr = DetectNewComputeShader(vecCS);
	if (FAILED(hr))
	{
		DEBUG_BREAK;
		return hr;
	}

	//코드 작성 시작
	Writer.WriteCode(0, define_Preset::Keyword::Head::A);
	Writer.WriteCode(0, define_Preset::Keyword::DefineSTRKEY::A);

	Writer.WriteCode(0);
	Writer.WriteCode(0);

	{
		std::string strCode;
		strCode +=  "namespace ";
		strCode += define_Preset::Keyword::strKey::A;
		strCode += "Shader";
		Writer.WriteCode(0, strCode);
		Writer.OpenBracket(0);
	}

	{
		std::string strCode;
		strCode += "namespace Graphics";
		Writer.WriteCode(0, strCode);
		Writer.OpenBracket(0);
	}

	for (const auto& iter : umapGSGroup)
	{
		std::string strCode;
		strCode +=  "STRKEY ";
		strCode += iter.first.filename().replace_extension("").string();
		strCode += " = \"";
		strCode += iter.first.string();
		strCode += "\";";
		Writer.WriteCode(0, strCode);
	}
	Writer.CloseBracket(0, true);

	{
		Writer.AddIndentation(1);
		std::string strCode;
		strCode += "namespace Compute";
		Writer.WriteCode(1, strCode);
		Writer.OpenBracket(1);
	}

	for (size_t i = 0; i < vecCS.size(); ++i)
	{
		std::string strCode;
		strCode += "STRKEY ";
		strCode += vecCS[i].filename().replace_extension("").string();
		strCode += " = \"";
		strCode += vecCS[i].filename().replace_extension(".json").string();
		strCode += "\";";
		Writer.WriteCode(1, strCode);
	}
	Writer.CloseBracket(1, true);
	Writer.CloseBracket(1);

	//Writer.CloseBracketAll(0);

	return Writer.SaveAll<char>(_FilePath);
}
