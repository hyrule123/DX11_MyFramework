#include "pch.h"

#include "MacroFunc.h"

void WriteCode(std::wofstream& _pFile, const wstring& _strCode)
{
	static int s_iIndentation = 0;

	//한줄 내려준다.
	_pFile << L"\n";

	//공란으로 넣을 경우 한줄 내리기만 하고 끝
	if (true == _strCode.empty())
		return;

	//닫는 중괄호가 있으면 먼저 값을 내려준다.
	if (std::wstring::npos != _strCode.find(BracketEnd))
	{
		--s_iIndentation;
		if (s_iIndentation < 0)
			s_iIndentation = 0;
	}

	//현재 들여쓰기 단계만큼 들여쓰기를 해준다.
	for (int i = 0; i < s_iIndentation; ++i)
		_pFile << L"\t";

	//중괄호가 있을 경우 indentation 증가
	if (std::wstring::npos != _strCode.find(BracketStart))
	{
		++s_iIndentation;
	}

	//코드 작성
	_pFile << _strCode;
}