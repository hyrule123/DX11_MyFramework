#include "pch.h"

#include "CodeGenFunc.h"


void CreateScriptCode(const string& _wstrPath)
{
	MessageBoxA(nullptr, "Script", NULL, MB_OK);


	//TODO : C스타일 작성으로 변경하기
	std::wofstream code;
	code.open("D:\\Users\\ekdrn\\DeskTop\\Test.cpp");
	if (false == code.is_open())
		return;

	code.write(L"#pragma once\n\n", lstrlen(L"#pragma once\n\n"));

	

	code.close();
}