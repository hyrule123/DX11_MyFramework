#include "pch.h"

#include "CodeGenFunc.h"
#include "MacroFunc.h"

constexpr const wchar_t* ScriptStandardPrefix = L"CScript_";
constexpr const wchar_t* ScriptStandardSuffix = L".h";

void CreateScriptCode()
{
	string ScriptPath = "./Project/Script/";

	//Key = strKey의 변수명, Value = 파일명
	unordered_map<wstring, wstring> mapScripts;

	std::filesystem::directory_iterator iter(ScriptPath);
	while (std::filesystem::end(iter) != iter)
	{
		const std::filesystem::directory_entry& entry = *iter;

		const wstring& ScriptVal = entry.path().filename().wstring();

		//"CScript_" 와 .h가 붙어있는 파일들만 추가
		if (wstring::npos != ScriptVal.find(ScriptStandardPrefix) && wstring::npos != ScriptVal.find(ScriptStandardSuffix))
		{
			wstring ScriptKey = ScriptVal;

			//앞부분 제거
			ScriptKey.erase(0, lstrlenW(ScriptStandardPrefix));

			//뒷부분 제거
			size_t pos = ScriptKey.find(ScriptStandardSuffix);
			if (wstring::npos != pos)
				ScriptKey.erase(pos, lstrlenW(ScriptStandardSuffix));

			//대문자로 변경
			transform(ScriptKey.begin(), ScriptKey.end(), ScriptKey.begin(), ::towupper);


			mapScripts.insert(make_pair(ScriptKey, ScriptVal));
		}

		++iter;
	}


	//strKey 작성
	string strKeyPath = ScriptPath + "strKeyScript.h";
	std::wofstream strKey(strKeyPath);
	if (false == strKey.is_open())
		return;

	WriteCode(strKey, L"#pragma once");
	WriteCode(strKey);

	WriteCode(strKey, L"//This Code is Automatically generated by CodeGenerator.exe");
	WriteCode(strKey);

	WriteCode(strKey, L"namespace SCRIPTS");
	WriteCode(strKey, L"{");

	
	const wstring& BaseCode = L"constexpr const char* ";
	for (const auto& iter : mapScripts)
	{
		wstring FinalCodeLine = BaseCode + iter.first + L" = ";
		FinalCodeLine += L"\"";
		FinalCodeLine += iter.second + L'"';
		FinalCodeLine += L";";
		WriteCode(strKey, FinalCodeLine);
	}

	WriteCode(strKey, L"}");

	strKey.close();


	//CScriptMgr 작성
	string ScriptMgrPath = ScriptPath + "CScriptMgr.cpp";
	std::wofstream ScriptMgr(ScriptMgrPath);
	if (false == ScriptMgr.is_open())
		return;

	//기본 include
	wstring RawLiteral = LR"(
#pragma once

#include "pch.h"
#include "CScriptMgr.h"

)";
	ScriptMgr << RawLiteral;


	//생성된 스크립트 순회돌며 삽입
	WriteCode(ScriptMgr, L"//Created Scripts");
	for (const auto& iter : mapScripts)
	{
		IncludeFile(ScriptMgr, iter.second);
	}
	WriteCode(ScriptMgr);


	//생성자 및 소멸자
	RawLiteral = LR"(


CScriptMgr::CScriptMgr() {}
CScriptMgr::~CScriptMgr()
{
	for(const auto& iter : m_umapScript)
	{
		DESTRUCTOR_DELETE(iter.second);
	}
}

)";
	ScriptMgr << RawLiteral;


	//init()
	WriteCode(ScriptMgr, L"void CScriptMgr::init()");
	WriteCode(ScriptMgr, L"{");
	
	for (const auto& iter : mapScripts)
	{
		wstring className = L"m_umapScript[";
		className += L"SCRIPTS::" + iter.first + L"] = new ";
		
		size_t pos = iter.second.find(ScriptStandardSuffix);
		className.append(iter.second.substr(0, pos));
		className += ';';

		WriteCode(ScriptMgr, className);
	}

	WriteCode(ScriptMgr, L"}");


	RawLiteral = LR"(

CScript* CScriptMgr::GetNewScript(const string& _strKey)
{
	const auto& iter = m_umapScript.find(_strKey);

	if (iter == m_umapScript.end())
		return nullptr;

	return iter->second->Clone();
}
)";

	ScriptMgr << RawLiteral;


	ScriptMgr.close();
}
