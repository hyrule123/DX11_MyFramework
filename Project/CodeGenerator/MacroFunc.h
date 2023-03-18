#pragma once

constexpr const wchar_t BracketStart = L'{';
constexpr const wchar_t BracketEnd = L'}';
constexpr const wchar_t* IncludeBegin = L"#include \"";

extern int g_iIndentation;


//아무 값도 입력하지 않을 경우 한 줄 내림
//반환값 : 현재 indent 단계
void WriteCodeA(ofstream& _pFile, const string& _strCode = "");
void WriteCodeW(wofstream& _pFile, const wstring& _wstrCode = L"");

inline void WriteBracketOpenA(ofstream& _pFile)
{
	//먼저 중괄호를 추가한뒤
	WriteCodeA(_pFile, "{");

	//들여쓰기 값을 늘려준다.
	++g_iIndentation;
}

inline void WriteBracketCloseA(ofstream& _pFile)
{
	//먼저 들여쓰기를 뺴준뒤
	--g_iIndentation;
	if (g_iIndentation < 0)
		g_iIndentation = 0;

	//WriteCode 호출
	WriteCodeA(_pFile, "}");
}

inline void WriteBracketCloseAllA(ofstream& _pFile)
{
	while (0 < g_iIndentation)
	{
		WriteBracketCloseA(_pFile);
	}
}

inline void WriteBracketOpenA(wofstream& _pFile)
{
	//먼저 중괄호를 추가한뒤
	WriteCodeW(_pFile, L"{");

	//들여쓰기 값을 늘려준다.
	++g_iIndentation;
}

inline void WriteBracketCloseW(wofstream& _pFile)
{
	//먼저 들여쓰기를 뺴준뒤
	--g_iIndentation;
	if (g_iIndentation < 0)
		g_iIndentation = 0;

	//WriteCode 호출
	WriteCodeW(_pFile, L"}");
}

inline void WriteBracketCloseAllW(wofstream& _pFile)
{
	while (0 < g_iIndentation)
	{
		WriteBracketCloseW(_pFile);
	}
}


inline void IncludeFile(std::wofstream& _pFile, const wstring& _strIncludeFileName)
{
	wstring include = IncludeBegin;
	include += _strIncludeFileName + L'\"';

	WriteCodeW(_pFile, include);
}
