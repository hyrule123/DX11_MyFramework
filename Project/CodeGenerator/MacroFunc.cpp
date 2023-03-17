#include "pch.h"

#include "MacroFunc.h"

void WriteCodeA(std::ofstream& _pFile, const string& _strCode)
{
	//한줄 내려준다.
	_pFile << "\n";

	//공란으로 넣을 경우 한줄 내리기만 하고 끝
	if (true == _strCode.empty())
		return;

	//현재 들여쓰기 단계만큼 들여쓰기를 해준다.
	for (int i = 0; i < g_iIndentation; ++i)
		_pFile << "\t";

	//코드 작성
	_pFile << _strCode;
	return;
}


void WriteCodeW(std::wofstream& _pFile, const wstring& _wstrCode)
{
	static int s_iIndentation = 0;

	//한줄 내려준다.
	_pFile << L"\n";

	//공란으로 넣을 경우 한줄 내리기만 하고 끝
	if (true == _wstrCode.empty())
		return;

	//닫는 중괄호가 있으면 먼저 값을 내려준다.
	if (std::wstring::npos != _wstrCode.find(BracketEnd))
	{
		--g_iIndentation;
		if (g_iIndentation < 0)
			g_iIndentation = 0;
	}

	//현재 들여쓰기 단계만큼 들여쓰기를 해준다.
	for (int i = 0; i < g_iIndentation; ++i)
		_pFile << L"\t";

	//중괄호가 있을 경우 indentation 증가
	if (std::wstring::npos != _wstrCode.find(BracketStart))
	{
		++g_iIndentation;
	}

	//코드 작성
	_pFile << _wstrCode;
	return;
}


