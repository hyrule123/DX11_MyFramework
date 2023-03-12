#pragma once

#include <UtilLib_DLL/forwards.h>
class CShaderLoader
{
public:
	CShaderLoader();
	~CShaderLoader();

private:
	Json::Value* m_jsonShaderInfo;
	bool m_bValueModified;

public:
	bool LoadJsonFile();
	bool LoadAllShaders();
	const Json::Value* GetShaderInfo(const string& _strShaderKey);

private:
	bool CreateDefaultGraphicsShader(const wstring& _wstrShaderBasePath, const Json::Value& _ShaderInfo);
	bool CreateDefaultComputeShader(const wstring& _wstrShaderBasePath, const Json::Value& _ShaderInfo);
};

