#include "pch.h"

#include "CodeGenFunc.h"

#include "MacroFunc.h"


#include "StringConvert.h"

#include <UtilLib_DLL/json/json.h>
#include <Engine/define.h>
#include <d3dcommon.h>

#include <Engine/CComputeShader.h>
#include <Engine/CGraphicsShader.h>

//순수가상함수 풀고 저장기능만 사용하기 위한 더미 클래스
class CDummyCompute
	: public CComputeShader
{
public:
	CDummyCompute() {};
	virtual ~CDummyCompute() {};

	virtual bool BindDataCS() {};
	virtual void UnBindCS() {};
};


struct tShaderSetting
{
	string ShaderBaseName;
	int flagPipelineStage;
};

void InitShaderSettingGraphics(Json::Value& _jVal, const tShaderSetting& _tShaderSetting);
void InitShaderSettingCompute(Json::Value& _jVal, const tShaderSetting& _tShaderSetting);

void CreateShaderCode()
{
	//first(Key) = json파일의 이름
	map<string, tShaderSetting> mapShaderInfo;

	filesystem::path ShaderDir(PresetPath::ContentPath);
	ShaderDir /= RES_INFO::SHADER::DirNameRoot;

	filesystem::path GSShaderDir = ShaderDir / RES_INFO::SHADER::GRAPHICS::DirName;

	//Content/Shader/Graphics 폴더 안의 파일을 순회돌아준다.
	filesystem::directory_iterator dirIter(GSShaderDir);
	filesystem::directory_iterator dirIterEnd = std::filesystem::end(dirIter);
	for (dirIter; dirIter != dirIterEnd; ++dirIter)
	{
		const filesystem::directory_entry& file = *dirIter;

		if (false == file.is_directory())
		{
			//파일명을 받아온다.
			string shaderName = file.path().filename().string();

			
			//확장자가 .cso로 끝나는지 확인한다. 확장자가 cso가 아닐 경우 쉐이더가 아니므로 continue;
			size_t strPos = shaderName.find(RES_INFO::SHADER::Extension_ShaderCode);
			if (string::npos == strPos)
				continue;

			//.cso 확장자를 제거한다.
			shaderName.erase(strPos, string::npos);

			//이름을 통해서 어떤 파이프라인인지를 유추한다.
			int flagPipeline = (int)eSHADER_PIPELINE_STAGE::__NONE;
			for (int i = 0; i < (int)eSHADER_TYPE::END; ++i)
			{
				if (string::npos != shaderName.find(RES_INFO::SHADER::GRAPHICS::PrefixArr[i]))
				{
					flagPipeline |= 1 << i;
					break;
				}
			}
			
			//마지막 "_" 위치를 찾아서 쉐이더 관련 정보를 지우고, 이름만 남겨준다.
			//Key 및 Value로 사용할 예정임.
			strPos = shaderName.find_last_of("_");
			if (string::npos != strPos)
			{
				//마지막 _ 위치까지 지워준다.(쉐이더의 이름만 남겨 준다.)
				shaderName.erase((size_t)0, strPos + (size_t)1);
			}
			
			//처음 발견한 쉐이더 이름일 경우 NONE 값을 넣고 초기화해준다.
			auto mapIter = mapShaderInfo.find(shaderName);
			if (mapIter == mapShaderInfo.end())
			{
				mapShaderInfo[shaderName] = tShaderSetting{ shaderName, eSHADER_PIPELINE_STAGE::__NONE };
				
				//새로 생성한 값을 iterator에 등록해준다.(아래에서도 사용함)
				mapIter = mapShaderInfo.find(shaderName);
				mapIter->second.ShaderBaseName += RES_INFO::SHADER::Extension_ShaderCode;
			}

			//발견한 쉐이더 파일의 파이프라인단계를 플래그값에 추가한다.
			mapIter->second.flagPipelineStage |= flagPipeline;
		}
	}
	//생성된 HLSL 파일 순회 끝



	bool bNewShaderDetected = false;
	//map을 순회돌아주면서 파일을 열어본다.
	//파일명은 'Key값'이다.
	Json::Value JsonInfo;
	for (const auto& mapIter : mapShaderInfo)
	{
		//JSON 경로 생성
		filesystem::path JsonPath = GSShaderDir;
		JsonPath /= mapIter.first;
		JsonPath += RES_INFO::SHADER::Extension_ShaderSetting;

		ifstream fpJsonIn(JsonPath);
		//파일이 열렸으면 데이터를 읽어온다.
		if (true == fpJsonIn.is_open())
		{
			fpJsonIn >> JsonInfo;
			fpJsonIn.close();
		}
		//파일이 없을 경우 JsonValue를 포맷에 맞게 초기화시킨다.
		//초기화될 때 이름은 자동으로 들어감.
		else
		{
			InitShaderSettingGraphics(JsonInfo, mapIter.second);

			//또한 새로운 쉐이더 파일이 발견되었을경우 값을 설정할수 있도록 MessageBox를 띄워준다.
			bNewShaderDetected = true;
		}

		//데이터 확인 후 쉐이더 파이프라인 단계에 차이점이 있을 경우 파이프라인 단계값을 갱신하고 파일로 내보낸다.
		//변경점이 없으면 파일로 내보내지 않는다.
		int origFlag = JsonInfo[string(RES_INFO::SHADER::GRAPHICS::Setting::ePipelineFlag)].asInt();
		
		if (mapIter.second.flagPipelineStage != origFlag)
		{
			ofstream fpJsonOut(JsonPath);
			if (true == fpJsonOut.is_open())
			{
				JsonInfo[string(RES_INFO::SHADER::GRAPHICS::Setting::ePipelineFlag)] = mapIter.second.flagPipelineStage;
				fpJsonOut << JsonInfo;
				fpJsonOut.close();
			}
		}
	}


	//새로운 쉐이더를 발견했을 경우 strKeysShader.h 파일도 다시 작성해준다.
	if (true == bNewShaderDetected)
	{
		std::ofstream fpStrKeyShader("./Project/Script/strKeyShader.h");
		if (true == fpStrKeyShader.is_open())
		{
			//머릿글프리셋 삽입
			string RawLiteral = string(PresetStr::Head);
			fpStrKeyShader << RawLiteral;

			WriteBracketOpenA(fpStrKeyShader);
			WriteCodeA(fpStrKeyShader, "namespace GRAPHICS");
			WriteBracketOpenA(fpStrKeyShader);
			
			for(const auto& GSIter : mapShaderInfo)
			{
				string codeLine = "constexpr const char* ";

				//JSON 파일 명
				string strKeyUpper;
				strKeyUpper.resize(GSIter.first.size());
				transform(GSIter.first.begin(), GSIter.first.end(), strKeyUpper.begin(), ::toupper);
				codeLine += strKeyUpper;
				codeLine += " = \"";

				//CSO 파일 명
				codeLine += GSIter.first + string(RES_INFO::SHADER::Extension_ShaderSetting) + "\";";

				WriteCodeA(fpStrKeyShader, codeLine);
			}
			WriteBracketCloseA(fpStrKeyShader);
			WriteCodeA(fpStrKeyShader);


			WriteCodeA(fpStrKeyShader, "namespace COMPUTE");
			WriteBracketOpenA(fpStrKeyShader);


			filesystem::path CSShaderDir(ShaderDir);
			CSShaderDir /= "Compute";
			
			dirIter = filesystem::directory_iterator(CSShaderDir);
			for(;false == dirIter._At_end(); ++dirIter)
			{	
				if (false == (*dirIter).is_directory())
				{
					string codeLine = "constexpr const char* ";

					
					string Key = (*dirIter).path().filename().string();

					string Value = Key;

					//앞의 _와 뒤의 확장자를 지우고 대문자로 만들어서 키값으로 변환해준다.
					size_t strPos = Key.find(RES_INFO::SHADER::Extension_ShaderCode);
					if (string::npos != strPos)
					{
						Key.erase(strPos, string::npos);
					}
					strPos = Key.find_last_of("_");
					if (string::npos != strPos)
					{
						Key.erase((size_t)0, strPos + (size_t)1);
					}

					transform(Key.begin(), Key.end(), Key.begin(), ::toupper);

					codeLine += Key;
					codeLine += " = \"";
					codeLine += Value + "\";";

					WriteCodeA(fpStrKeyShader, codeLine);
				}
			}
			WriteBracketCloseA(fpStrKeyShader);

			WriteBracketCloseA(fpStrKeyShader);
			WriteCodeA(fpStrKeyShader);

			fpStrKeyShader.close();
		}


		MessageBoxA(nullptr, "New Shader Detected.\nDo not close this message box until adjust the values.", "Notification", MB_OK);
	}


}


void InitShaderSettingGraphics(Json::Value& _jVal, const tShaderSetting& _tShaderSetting)
{
	_jVal.clear();

	CGraphicsShader dummy;
	dummy.SetName(_tShaderSetting.ShaderBaseName);
	dummy.SaveJson(&_jVal);

	//_jVal[string(RES_INFO::SHADER::GRAPHICS::Setting::ShaderBaseName)] = "";

	//_jVal[string(RES_INFO::SHADER::GRAPHICS::Setting::ePipelineFlag)] = eSHADER_PIPELINE_STAGE::__NONE;

	//_jVal[string(RES_INFO::SHADER::GRAPHICS::Setting::eTopology)] = (int)D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	//_jVal[string(RES_INFO::SHADER::GRAPHICS::Setting::eRSState)] = (int)eRASTERIZER_TYPE::CULL_BACK;
	//_jVal[string(RES_INFO::SHADER::GRAPHICS::Setting::eDSState)] = (int)eDEPTHSTENCIL_TYPE::LESS;
	//_jVal[string(RES_INFO::SHADER::GRAPHICS::Setting::eBState)] = (int)eBLENDSTATE_TYPE::DEFAULT;
	//_jVal[string(RES_INFO::SHADER::GRAPHICS::Setting::eShaderDomain)] = (int)eSHADER_DOMAIN::_UNDEFINED;
}


void InitShaderSettingCompute(Json::Value& _jVal, const tShaderSetting& _tShaderSetting)
{
	_jVal.clear();
}
