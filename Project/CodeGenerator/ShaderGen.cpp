#include "pch.h"

#include "CodeGenFunc.h"

#include "MacroFunc.h"


#include "StringConvert.h"

#include <UtilLib_DLL/json/json.h>
#include <Engine/define.h>
#include <d3dcommon.h>

#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_Debug")
#else
#pragma comment(lib, "Engine/Engine_Release")
#endif

#include <Engine/CComputeShader.h>
#include <Engine/CGraphicsShader.h>

//순수가상함수 풀고 저장기능만 사용하기 위한 더미 클래스
class CDummyCompute
	: public CComputeShader
{
public:
	CDummyCompute() {};
	virtual ~CDummyCompute() {};

	virtual bool BindDataCS() { return true; };
	virtual void UnBindCS() {};
};


struct tShaderSetting
{
	string ShaderCodeName;
	int flagPipelineStage;
};

void InitShaderSettingGraphics(Json::Value& _jVal, const pair<string, tShaderSetting>& _mapPair);
void InitShaderSettingCompute(Json::Value& _jVal, const filesystem::path& _filePath);

void CreateShaderCode()
{
	//first(Key) = json파일의 이름
	map<string, tShaderSetting> mapShaderInfo;

	filesystem::path ShaderDir(PresetPath::Content);
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
			filesystem::path SFileName = file.path().lexically_relative(GSShaderDir);

			//확장자가 .cso로 끝나는지 확인한다. 확장자가 cso가 아닐 경우 쉐이더가 아니므로 continue;
			if (SFileName.extension() != RES_INFO::SHADER::Extension_ShaderCode)
				continue;

			SFileName.replace_extension("");

			string SFileNameBase = SFileName.string();

			//이름을 통해서 어떤 파이프라인인지를 유추한다.
			int flagPipeline = (int)eSHADER_PIPELINE_STAGE::__NONE;
			for (int i = 0; i < (int)eSHADER_TYPE::END; ++i)
			{
				if (string::npos != SFileNameBase.find(RES_INFO::SHADER::GRAPHICS::PrefixArr[i]))
				{
					flagPipeline |= 1 << i;
					break;
				}
			}

			//마지막 "_" 위치를 찾아서 쉐이더 관련 정보를 지우고, 이름만 남겨준다.
			//Key 및 Value로 사용할 예정임.
			size_t strPos = SFileNameBase.find_last_of("_");
			if (string::npos != strPos)
			{
				//마지막 _ 위치까지 지워준다.(쉐이더의 이름만 남겨 준다.)
				SFileNameBase.erase((size_t)0, strPos + (size_t)1);
			}

			std::filesystem::path FileName = SFileNameBase;
			FileName.replace_extension(RES_INFO::SHADER::Extension_ShaderSetting);
			string strFileName = FileName.string();
			//처음 발견한 쉐이더 이름일 경우 NONE 값을 넣고 초기화해준다.
			auto mapIter = mapShaderInfo.find(strFileName);
			if (mapIter == mapShaderInfo.end())
			{
				string CodeFileName = FileName.replace_extension(RES_INFO::SHADER::Extension_ShaderCode).string();

				mapShaderInfo[strFileName] = tShaderSetting{CodeFileName, eSHADER_PIPELINE_STAGE::__NONE};

				//새로 생성한 값을 iterator에 등록해준다.(아래에서도 사용함)
				mapIter = mapShaderInfo.find(strFileName);
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
			InitShaderSettingGraphics(JsonInfo, mapIter);

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





	filesystem::path filePath(PresetPath::ScriptProj);
	filePath /= PresetPath::strKeyShader;
	std::ofstream fpStrKeyShader(filePath);
	if (true == fpStrKeyShader.is_open())
	{
		//머릿글프리셋 삽입
		string RawLiteral = string(PresetStr::Head);
		fpStrKeyShader << RawLiteral;

		WriteCodeA(fpStrKeyShader, "namespace SHADER");
		WriteBracketOpenA(fpStrKeyShader);
		WriteCodeA(fpStrKeyShader, "namespace GRAPHICS");
		WriteBracketOpenA(fpStrKeyShader);
			
		for(const auto& GSIter : mapShaderInfo)
		{
			string codeLine = string(PresetStr::ConstexprStringView);

			//JSON 파일 명
			string strKeyUpper = GSIter.first;
			size_t pos = strKeyUpper.find('.');
			if (string::npos != pos)
				strKeyUpper.erase(pos, string::npos);
			transform(strKeyUpper.begin(), strKeyUpper.end(), strKeyUpper.begin(), ::toupper);
			codeLine += strKeyUpper;
			codeLine += " = \"";

			//CSO 파일 명
			codeLine += GSIter.first + "\";";

			WriteCodeA(fpStrKeyShader, codeLine);
		}
		WriteBracketCloseA(fpStrKeyShader);
		WriteCodeA(fpStrKeyShader);


		WriteCodeA(fpStrKeyShader, "namespace COMPUTE");
		WriteBracketOpenA(fpStrKeyShader);


		filesystem::path CSShaderDir(ShaderDir);
		CSShaderDir /= "Compute";
		try
		{
			dirIter = filesystem::directory_iterator(CSShaderDir);
			for (; false == dirIter._At_end(); ++dirIter)
			{
				if (false == (*dirIter).is_directory())
				{
					//cso 파일을 읽었을 때만 진행.
					filesystem::path FileName = dirIter->path();
					if (0 != FileName.extension().compare(RES_INFO::SHADER::Extension_ShaderCode))
						continue;

					FileName.replace_extension(RES_INFO::SHADER::Extension_ShaderSetting);

					//파일이 존재하면 스킵
					ifstream ifpSetting(FileName);



					if (true == ifpSetting.is_open())
						ifpSetting.close();
					
					//파일이 존재하지 않을 시 파일을 생성
					else
					{
						ofstream outFile(FileName);

						if (outFile.is_open())
						{
							Json::Value ShaderVal;
							InitShaderSettingCompute(ShaderVal, FileName.lexically_relative(CSShaderDir));

							outFile << ShaderVal;

							outFile.close();
						}
					}


					string codeLine = string(PresetStr::ConstexprStringView);



					string Key = (*dirIter).path().filename().replace_extension().string();

					string Value = Key;


					size_t strPos = Key.find_last_of("_");
					if (string::npos != strPos)
					{
						Key.erase((size_t)0, strPos + (size_t)1);
					}

					transform(Key.begin(), Key.end(), Key.begin(), ::toupper);

					codeLine += Key;
					codeLine += " = \"";
					codeLine += Value + string(RES_INFO::SHADER::Extension_ShaderSetting) + "\";";

					WriteCodeA(fpStrKeyShader, codeLine);
				}
			}
			WriteBracketCloseA(fpStrKeyShader);

			WriteBracketCloseA(fpStrKeyShader);
			WriteCodeA(fpStrKeyShader);

			fpStrKeyShader.close();
		}
		catch (const std::filesystem::filesystem_error& error)
		{
			MessageBoxA(nullptr, error.what(), NULL, MB_OK);
			fpStrKeyShader.close();
			throw(error);
		}

	}


	if (true == bNewShaderDetected)
	{
		MessageBoxA(nullptr, "New Shader Detected.\nDo not close this message box until adjust the values.", "Notification", MB_OK);
	}


}


void InitShaderSettingGraphics(Json::Value& _jVal, const pair<string, tShaderSetting>& _mapPair)
{
	_jVal.clear();

	CGraphicsShader dummy;

	//파이프라인 정보는 바깥에서 지정할 예정(값을 전달할 방법이 없음)
	dummy.SetName(_mapPair.second.ShaderCodeName);
	dummy.SetRelativePath(_mapPair.first);
	dummy.SetKey(_mapPair.first);

	//이쪽 코드에서 추가로 설정작업을 해줘야 하기 때문에 포인터를 통해 값을 받아오는 방식을 사용
	dummy.SaveJson(&_jVal);
}


void InitShaderSettingCompute(Json::Value& _jVal, const filesystem::path& _filePath)
{
	filesystem::path CodeFileName = _filePath;
	CodeFileName.replace_extension(RES_INFO::SHADER::Extension_ShaderCode);

	CDummyCompute dummy;

	dummy.SetName(CodeFileName.string());
	dummy.SetRelativePath(_filePath);
	dummy.SetKey(_filePath.string());


	dummy.SaveJson(&_jVal);
}
