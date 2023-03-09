#include "pch.h"

#include "CodeGenFunc.h"

#include "MacroFunc.h"

#include "json-cpp/json.h"

void CreateShaderCode()
{
	string ShaderDir = "OutputFile\\Bin_";
#ifdef _DEBUG
	ShaderDir += "Debug\\";
#else
	ShaderDir += "Release\\";
#endif
	ShaderDir += "Content\\Shader\\";

	string JsonPath = ShaderDir + "ShaderMap.json";

	Json::Value jsonfile;

	std::filesystem::directory_iterator iter(ShaderDir);
	while (std::filesystem::end(iter) != iter)
	{
		const std::filesystem::directory_entry& entry = *iter;

		//jsoncpp ÀúÀå¿ë
		string utf8filename = entry.path().filename().string();
		wstring wfilename = entry.path().filename().wstring();

		++iter;
	}


}