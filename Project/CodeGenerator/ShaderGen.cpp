#include "pch.h"

#include "CodeGenFunc.h"

#include "MacroFunc.h"

#include "json-cpp/json.h"

#include "StringConvert.h"

#include <Engine/define.h>
#include <d3dcommon.h>

void InitializeShaderInfoJson(Json::Value& _jVal);

void CreateShaderCode()
{
	map<string, int> mapShaderInfo;

	filesystem::path ShaderDir(u8"./OutputFile/Bin_");
#ifdef _DEBUG
	ShaderDir += "Debug";
#else
	ShaderDir += "Release";
#endif
	ShaderDir /= "Content/Shader/";

	filesystem::directory_iterator dirIter(ShaderDir);
	const auto& dirIterEnd = std::filesystem::end(dirIter);
	for (dirIter; dirIter != dirIterEnd; ++dirIter)
	{
		const filesystem::directory_entry& file = *dirIter;

		if (false == file.is_directory())
		{
			string origFileName = file.path().filename().string();
			string shaderName = origFileName;

			//확장자가 .cso로 끝나는지 확인한다. 확장자가 cso가 아닐 경우 continue;
			size_t strPos = shaderName.find(JSON_SHADERINFO::ShaderExtension);
			if (string::npos == strPos)
				continue;
			//우선 ".cso"를 지워준다.
			shaderName.erase(strPos);

			//마지막 "_" 위치를 찾아서 쉐이더 관련 정보를 지우고, 이름만 남겨준다.
			//Key로 사용할 예정.
			strPos = shaderName.find_last_of("_");
			if (string::npos != strPos)
			{
				//마지막 _ 위치까지 지워준다.(쉐이더의 이름만 남겨 준다.)
				shaderName.erase((size_t)0, (size_t)(strPos + 1));
			}

			//뒤에 json 확장자를 붙여준다.
			shaderName += ".json";

			//앞에 S_I_를 붙여준다.
			shaderName = "S_Info_" + shaderName;

			//처음 발견한 쉐이더 이름일 경우 NONE 값을 넣고 초기화해준다.
			auto mapIter = mapShaderInfo.find(shaderName);
			if (mapIter == mapShaderInfo.end())
			{
				mapShaderInfo[shaderName] = eSHADER_PIPELINE_STAGE::__NONE;

				//새로 생성한 값을 iterator에 등록해준다.
				mapIter = mapShaderInfo.find(shaderName);
			}


			//쉐이더 분류를 확인한다.
			//컴퓨트쉐이더인지 확인하고 분류에 맞는 쉐이더 트리의 주소를 저장해놓는다.
			//컴퓨트쉐이더와 그래픽쉐이더는 로드할 클래스가 아예 다르므로 중복되면 안된다.
			if (string::npos != origFileName.find(JSON_SHADERINFO::ComputeShaderName)) //컴퓨트쉐이더일 경우
			{
				mapIter->second = eSHADER_PIPELINE_STAGE::__COMPUTE;
			}

			else //그래픽 쉐이더일 경우: 파이프라인 플래그값도 계산해줘야 함.
			{
				for (int i = 0; i < (int)eSHADER_TYPE::END; ++i)
				{
					if (string::npos != origFileName.find(JSON_SHADERINFO::arrShaderName[i]))
					{
						mapIter->second |= 1 << i;
					}
				}

				//if (string::npos != shaderName.find(JSON_SHADERINFO::arrShaderName[(int)eSHADER_TYPE::__VERTEX]))
				//	mapIter->second |= eSHADER_PIPELINE_STAGE::__VERTEX;
				//else if (string::npos != shaderName.find(JSON_SHADERINFO::arrShaderName[(int)eSHADER_TYPE::__HULL]))
				//	mapIter->second |= eSHADER_PIPELINE_STAGE::__HULL;
				//else if (string::npos != shaderName.find(JSON_SHADERINFO::arrShaderName[(int)eSHADER_TYPE::__DOMAIN]))
				//	mapIter->second |= eSHADER_PIPELINE_STAGE::__DOMAIN;
				//else if (string::npos != shaderName.find(JSON_SHADERINFO::arrShaderName[(int)eSHADER_TYPE::__GEOMETRY]))
				//	mapIter->second |= eSHADER_PIPELINE_STAGE::__GEOMETRY;
				//else if (string::npos != shaderName.find(JSON_SHADERINFO::arrShaderName[(int)eSHADER_TYPE::__PIXEL]))
				//	mapIter->second |= eSHADER_PIPELINE_STAGE::__PIXEL;
			}

		}
	}


	////파일명으로 경로를 만들고, 열어본다.(파일이 존재하는지 확인한다)
	//filesystem::path pathShaderInfo(ShaderDir);
	//pathShaderInfo /= origFileName;
	//ifstream fpShaderInfo(pathShaderInfo);
	//if (true == fpShaderInfo.is_open())
	//{

	//}

	//map을 순회돌아주면서 파일을 열어본다.
	//파일명은 'Key값'이다.
	for (const auto& mapIter : mapShaderInfo)
	{
		//JSON 경로 생성
		filesystem::path JsonPath = ShaderDir;
		JsonPath /= mapIter.first;

		fstream fpJson(JsonPath);
		//파일이 열렸으면
		if (true == fpJson.is_open())
		{
			Json::Value JsonInfo;
			fpJson >> JsonInfo;


			JsonInfo[JSON_SHADERINFO::PipelineFlag]
		}
		
	}



	//새로운 쉐이더 발견 여부
	bool bNewShaderDetected = false;

	//저장 필요 여부
	bool bNeedSave = false;

	//파일이 있을 경우 파일 데이터를 가져온다.
	std::ifstream JsonFile(JsonPath);
	if (true == JsonFile.is_open())
	{
		JsonFile >> JsonInfo;
		JsonFile.close();
	}

	//분류가 하나라도 생성되어있지 않을 경우 값을 생성한다.
	if(false == JsonInfo.isMember(JSON_SHADERINFO::GraphicsShader))
		JsonInfo[JSON_SHADERINFO::GraphicsShader] = Json::Value(Json::ValueType::objectValue);
	if(false == JsonInfo.isMember(JSON_SHADERINFO::ComputeShader))
		JsonInfo[JSON_SHADERINFO::ComputeShader] = Json::Value(Json::ValueType::objectValue);



	////쉐이더 디렉토리를 순회돌아 준다.
	//std::filesystem::directory_iterator dirIter(ShaderDir);
	//const auto& iterEnd = std::filesystem::end(dirIter);
	//for (dirIter; iterEnd != dirIter; ++dirIter)
	//{
	//	const std::filesystem::directory_entry& entry = *dirIter;

	//	//jsoncpp 저장을 위해서 일반 string 타입으로 받아옴.(유니코드를 지원하지 않음)
	//	string strShaderName = entry.path().filename().string();

	//	//확장자가 .cso로 끝나는지 확인한다. 확장자가 cso가 아닐 경우 continue;
	//	size_t strPos = strShaderName.find(JSON_SHADERINFO::ShaderExtension);
	//	if (string::npos == strPos)
	//		continue;


	//	//데이터를 전달할 변수 선언.
	//	bool bIsGraphicShader = true;
	//	Json::Value* pShaderTree = nullptr;
	//	int flagShaderPipeline = 0;


	//	//우선 ".cso"를 지워준다.
	//	strShaderName.erase(strPos);


	//	//컴퓨트쉐이더인지 확인하고 분류에 맞는 쉐이더 트리의 주소를 저장해놓는다.
	//	strPos = strShaderName.find(JSON_SHADERINFO::ComputeShaderName);
	//	if (string::npos != strPos) //컴퓨트쉐이더일 경우
	//	{
	//		bIsGraphicShader = false;
	//		pShaderTree = &(JsonInfo[JSON_SHADERINFO::ComputeShader]);
	//	}


	//	else //그래픽 쉐이더일 경우: 파이프라인 플래그값도 계산해줘야 함.
	//	{
	//		pShaderTree = &(JsonInfo[JSON_SHADERINFO::GraphicsShader]);
	//			
	//		if (string::npos != strShaderName.find(JSON_SHADERINFO::VertexShaderName))
	//			flagShaderPipeline = eSHADER_PIPELINE_STAGE::__VERTEX;
	//		else if (string::npos != strShaderName.find(JSON_SHADERINFO::HullShaderName))
	//			flagShaderPipeline = eSHADER_PIPELINE_STAGE::__HULL;
	//		else if (string::npos != strShaderName.find(JSON_SHADERINFO::DomainShaderName))
	//			flagShaderPipeline = eSHADER_PIPELINE_STAGE::__DOMAIN;
	//		else if (string::npos != strShaderName.find(JSON_SHADERINFO::GeometryShaderName))
	//			flagShaderPipeline = eSHADER_PIPELINE_STAGE::__GEOMETRY;
	//		else if (string::npos != strShaderName.find(JSON_SHADERINFO::PixelShaderName))
	//			flagShaderPipeline = eSHADER_PIPELINE_STAGE::__PIXEL;
	//	}
	//			
	//	//예외 처리
	//	if (nullptr == pShaderTree)
	//		continue;


	//	//마지막 "_" 위치를 찾는다.
	//	strPos = strShaderName.find_last_of("_");
	//	if (string::npos != strPos)
	//	{
	//		//마지막 _ 위치까지 지워준다.(쉐이더의 이름만 남겨 준다.)
	//		strShaderName.erase((size_t)0, (size_t)(strPos + 1));
	//	}


	//	//노드 추가
	//	string strKey;
	//	strKey.resize(strShaderName.size());
	//	transform(strShaderName.begin(), strShaderName.end(), strKey.begin(), ::toupper);

	//	
	//	
	//	
	//	//JsonInfo에 들어있지 않았을 경우 새 쉐이더를 발견한 것임.
	//	//일단 데이터를 초기화해준다.
	//	if (false == pShaderTree->isMember(strKey))
	//	{
	//		bNewShaderDetected = true;
	//		bNeedSave = true;

	//		(*pShaderTree)[strKey] = Json::Value(Json::ValueType::objectValue);

	//		Json::Value& val = (*pShaderTree)[strKey];
	//		val[JSON_SHADERINFO::shaderName] = strShaderName;

	//		if (true == bIsGraphicShader)
	//		{
	//			//constexpr const char* shaderName = "shaderName";
	//			//constexpr const char* PipelineFlag = "PipelineFlag";
	//			//constexpr const char* Topology = "Topology";
	//			//constexpr const char* RSState = "Rasterizer";
	//			//constexpr const char* DSState = "DepthStencil";
	//			//constexpr const char* BState = "BlendState";
	//			//constexpr const char* ShaderDomain = "ShaderDomain";
	//			val[JSON_SHADERINFO::PipelineFlag] = flagShaderPipeline;
	//			val[JSON_SHADERINFO::Topology] = (int)D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	//			val[JSON_SHADERINFO::RSState] = (int)eRASTERIZER_TYPE::CULL_BACK;
	//			val[JSON_SHADERINFO::DSState] = (int)eDEPTHSTENCIL_TYPE::LESS;
	//			val[JSON_SHADERINFO::BState] = (int)eBLENDSTATE_TYPE::DEFAULT;
	//			val[JSON_SHADERINFO::ShaderDomain] = (int)eSHADER_DOMAIN::_UNDEFINED;
	//		}
	//		else
	//		{
	//			//val[JSON_SHADERINFO::PipelineFlag] = flagShaderPipeline;
	//		}
	//	}

	//	else //기존에 저장되어있는 값이 있을 경우
	//	{
	//		if (true == bIsGraphicShader)
	//		{
	//			int curFlag = (*pShaderTree)[strKey][JSON_SHADERINFO::PipelineFlag].asInt();
	//			//파이프라인 스테이지를 갱신해야 할 경우에만 저장이 필요하다고 알림
	//			if (false == (flagShaderPipeline & curFlag))
	//			{
	//				bNeedSave = true;
	//				curFlag |= flagShaderPipeline;
	//				(*pShaderTree)[strKey][JSON_SHADERINFO::PipelineFlag] = curFlag;
	//			}
	//		}
	//		else
	//		{

	//		}
	//	}
	//}



	//Json::Value::iterator iter = JsonInfo.begin();
	//Json::Value::iterator iterEnd = JsonInfo.end();
	//for (iter; iter != iterEnd; ++iter)
	//{

	//	if (true == iter->isMember("Filename"))
	//	{
	//		string Filename = (*iter)["Filename"].asString();

	//		int asdf = 0;
	//	}


	//	int a = 0;
	//}


	
	if (true == bNeedSave)
	{
		ofstream fout(JsonPath);
		if (true == fout.is_open())
		{
			fout << JsonInfo;
			fout.close();
		}

		if (true == bNewShaderDetected)
		{
			std::wofstream fpStrKeyShader("Project\\Script\\strKeyShader.h");
			if (true == fpStrKeyShader.is_open())
			{
				wstring RawLiteral = LR"(
#pragma once

//This Code is Automatically generated by CodeGenerator.exe

namespace SHADERS
)";
				fpStrKeyShader << RawLiteral;

				WriteCode(fpStrKeyShader, L"{");
				WriteCode(fpStrKeyShader, L"namespace GRAPHICS");
				WriteCode(fpStrKeyShader, L"{");
				
				const Json::Value& GSTree = JsonInfo[JSON_SHADERINFO::GraphicsShader];
				
				const auto& GSiterEnd = GSTree.end();
				for (auto GSiter = GSTree.begin(); GSiter != GSiterEnd; ++GSiter)
				{
					wstring codeLine = L"constexpr const char* ";
					codeLine += ConvertMultibyteToUnicode(GSiter.key().asString());
					codeLine += L" = \"";
					codeLine += ConvertMultibyteToUnicode((*GSiter)[JSON_SHADERINFO::ShaderName].asString()) + L"\";";

					WriteCode(fpStrKeyShader, codeLine);
				}
				WriteCode(fpStrKeyShader, L"}");
				WriteCode(fpStrKeyShader);

				

				WriteCode(fpStrKeyShader, L"namespace COMPUTE");
				WriteCode(fpStrKeyShader, L"{");

				const Json::Value& CSTree = JsonInfo[JSON_SHADERINFO::ComputeShader];

				const auto& CSiterEnd = CSTree.end();
				for (auto CSiter = CSTree.begin(); CSiter != CSiterEnd; ++CSiter)
				{
					wstring codeLine = L"constexpr const char* ";
					codeLine += ConvertMultibyteToUnicode(CSiter.key().asString());
					codeLine += L" = \"";
					codeLine += ConvertMultibyteToUnicode((*CSiter)[JSON_SHADERINFO::ShaderName].asString()) + L"\";";

					WriteCode(fpStrKeyShader, codeLine);
				}
				WriteCode(fpStrKeyShader, L"}");

				WriteCode(fpStrKeyShader, L"}");
				WriteCode(fpStrKeyShader);

				fpStrKeyShader.close();
			}





			MessageBoxA(nullptr, "New Shader Detected.\nDo not close this message box until adjust the values.", "Notification", MB_OK);
		}
	}

}


void InitializeShaderInfoJson(Json::Value& _jVal, bool _bIsGraphicsShader)
{
	_jVal.clear();

	if (true == _bIsGraphicsShader)
	{
		//constexpr const char* shaderName = "shaderName";
		//constexpr const char* PipelineFlag = "PipelineFlag";
		//constexpr const char* Topology = "Topology";
		//constexpr const char* RSState = "Rasterizer";
		//constexpr const char* DSState = "DepthStencil";
		//constexpr const char* BState = "BlendState";
		//constexpr const char* ShaderDomain = "ShaderDomain";
		_jVal[JSON_SHADERINFO::C]

		_jVal[JSON_SHADERINFO::PipelineFlag] = eSHADER_PIPELINE_STAGE::__NONE;
		_jVal[JSON_SHADERINFO::Topology] = (int)D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		_jVal[JSON_SHADERINFO::RSState] = (int)eRASTERIZER_TYPE::CULL_BACK;
		_jVal[JSON_SHADERINFO::DSState] = (int)eDEPTHSTENCIL_TYPE::LESS;
		_jVal[JSON_SHADERINFO::BState] = (int)eBLENDSTATE_TYPE::DEFAULT;
		_jVal[JSON_SHADERINFO::ShaderDomain] = (int)eSHADER_DOMAIN::_UNDEFINED;
	}
	else
	{
		_jVal[JSON_SHADERINFO:]
	}
}
